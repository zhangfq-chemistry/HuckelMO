
#include <QClipboard>
#include <QApplication>
#include <QProcess>
#include <QMessageBox>
#include <QCoreApplication>
#include <QDir>
#include "EHMO.h"
#include "HMO.h"

#include <cstdlib>
#include <stdio.h>
#include "huckelTextForm.h"
#include "mainwindow.h"
#include <QTextStream>
#include <QMap>
#include <array>
#include <QDebug>
#include <limits>
#include <QElapsedTimer>
#include <algorithm>
#include <cmath>

#include "eigenMathApi.h"

#if __has_include(<gmp.h>)
  #define BIGNUM_HAS_ARBITRARY_PRECISION_FLOAT 1
  #include "polyfactor/lll_gs.h"
  #include "polyfactor/lll_functions.h"
#endif

inline void bignum_clear_deadline() {}
inline void bignum_set_deadline_after_ms(qint64) {}

static const QRegularExpression RE_WHITESPACE("\\s+");

#ifdef BIGNUM_HAS_ARBITRARY_PRECISION_FLOAT
static QString mpzToQString(const mpz_t value)
{
    return QString::fromLatin1(mpz_get_str(nullptr, 10, value));
}
#endif

// Polynomial with arbitrary precision for large degree (up to x^60)
// Uses long long coefficients - values can get large but should fit for Hückel
class Poly {
public:
    std::vector<long long> c;  // c[i] = coefficient of x^i
    
    Poly() : c(1, 0) {}
    explicit Poly(int deg) : c(deg + 1, 0) {}
    Poly(std::initializer_list<long long> coeffs) : c(coeffs) { normalize(); }
    
    void normalize() { while (c.size() > 1 && c.back() == 0) c.pop_back(); }
    int deg() const { return static_cast<int>(c.size()) - 1; }
    
    long long& operator[](int i) { if (i >= (int)c.size()) c.resize(i + 1, 0); return c[i]; }
    long long operator[](int i) const { return (i < (int)c.size()) ? c[i] : 0; }
    
    Poly operator+(const Poly& o) const {
        Poly r(std::max(deg(), o.deg()));
        for (int i = 0; i <= deg(); i++) r[i] = c[i];
        for (int i = 0; i <= o.deg(); i++) r[i] += o[i];
        r.normalize(); return r;
    }
    Poly operator-(const Poly& o) const {
        Poly r(std::max(deg(), o.deg()));
        for (int i = 0; i <= deg(); i++) r[i] = c[i];
        for (int i = 0; i <= o.deg(); i++) r[i] -= o[i];
        r.normalize(); return r;
    }
    Poly operator*(long long k) const {
        Poly r = *this; for (auto& x : r.c) x *= k;
        r.normalize(); return r;
    }
    
    QString toString() const {
        if (deg() < 0 || (deg() == 0 && c[0] == 0)) return "0";
        QString s; bool first = true;
        for (int i = deg(); i >= 0; i--) {
            if (c[i] == 0) continue;
            if (!first && c[i] > 0) s += "+";
            if (i == 0) s += QString::number(c[i]);
            else if (i == 1) s += (c[i] == 1) ? "x" : (c[i] == -1) ? "-x" : QString::number(c[i]) + "x";
            else s += (c[i] == 1) ? QString("x^%1").arg(i) : 
                      (c[i] == -1) ? QString("-x^%1").arg(i) : QString("%1x^%2").arg(c[i]).arg(i);
            first = false;
        }
        return s.isEmpty() ? "0" : s;
    }
};

// Compute characteristic polynomial det(xI - A) using Faddeev-LeVerrier algorithm
// Complexity O(n³) - can handle n=60 efficiently
// Returns polynomial det(xI - A) = x^n + e[1]*x^(n-1) + ... + e[n]
Poly characteristicPoly(std::vector<std::vector<int>>& adj, int n) {
    if (n == 0) { Poly p; p[0] = 1; return p; }
    if (n == 1) { Poly p(1); p[1] = 1; p[0] = -adj[0][0]; return p; }
    
    // Compute traces of powers: t[k] = trace(A^k) for k=1..n
    // Use __int128 for exact integer arithmetic
    std::vector<std::vector<__int128>> A(n, std::vector<__int128>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            A[i][j] = adj[i][j];
    
    std::vector<__int128> t(n + 1, 0);
    std::vector<std::vector<__int128>> Ak = A;
    
    for (int k = 1; k <= n; k++) {
        // Compute trace of Ak
        __int128 trace = 0;
        for (int i = 0; i < n; i++) trace += Ak[i][i];
        t[k] = trace;
        
        if (k < n) {
            // Ak+1 = Ak * A
            std::vector<std::vector<__int128>> Anext(n, std::vector<__int128>(n, 0));
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    for (int l = 0; l < n; l++)
                        Anext[i][j] += Ak[i][l] * A[l][j];
            Ak = Anext;
        }
    }
    
    // Newton's identities: det(λI - A) = λ^n + e[1]*λ^(n-1) + ... + e[n]
    // e[k] = -1/k * (t[k] + e[1]*t[k-1] + ... + e[k-1]*t[1])
    std::vector<__int128> e(n + 1, 0);
    
    for (int k = 1; k <= n; k++) {
        __int128 sum = t[k];
        for (int j = 1; j < k; j++) {
            sum += e[j] * t[k - j];
        }
        e[k] = -sum / k;
    }
    
    // Build result: x^n + e[1]*x^(n-1) + ... + e[n]
    Poly result(n);
    result[n] = 1;
    for (int i = 1; i <= n; i++) {
        // Convert __int128 to long long, check for overflow
        if (e[i] > std::numeric_limits<long long>::max() || e[i] < std::numeric_limits<long long>::min()) {
            // Overflow, but for Hückel matrices this shouldn't happen for reasonable n
            // Use approximation or report error
            std::cerr << "Warning: coefficient overflow for i=" << i << std::endl;
        }
        result[n - i] = static_cast<long long>(e[i]);
    }
    return result;
}


HuckelTextForm::HuckelTextForm(QString title, QWidget *parent) :    QDialog(parent)
{
    ui.setupUi(this);

    if(title.size()<1)  setWindowTitle("Huckel-MO method");
    else   setWindowTitle(title);

    ui.textEdit->setLineWrapMode(QTextEdit::NoWrap);
    ui.textEdit->setCurrentFont(QFont("Courier", 20));
    ui.textEdit->setTextBackgroundColor(Qt::black);

    ui.textEdit->setStyleSheet("QTextEdit {background-color: #000; color: #ccc; font-family: monospace;}");

    charge=0;
    multipilicty=1;
    ui.spinBox_charge->setRange(-9,9);
    ui.spinBox_mulplicity->setValue(1);

    isHuckelCal=isXTBCal=isExtendedHuckelCal=false;
    runShellProcess=nullptr;
    //ehmo=nullptr;
    isTightBinding=false;
}


HuckelTextForm::~HuckelTextForm()
{
    if(runShellProcess)
        delete runShellProcess;
}


void HuckelTextForm::setParent (MainWindow * p)
{
    m_parent=p;
    mol=m_parent->getMol();
    view3d=m_parent->getView3D();
    ehmo=m_parent->getEHMO();
    hmo=m_parent->getHMO();
    setWindowFlags(Qt::Window | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
    initial();
}

void HuckelTextForm::initial()
{
    if(isHuckelCal)
        initialHuckel();

    if(isXTBCal)
        initialXTB();


    if(isExtendedHuckelCal)
        initialExtendedHuckel();
}



void HuckelTextForm::initialExtendedHuckel()
{
    qDebug() << "DEBUG: initialExtendedHuckel() started";
    qDebug() << "DEBUG: mol pointer =" << mol;
    if (mol) {
        qDebug() << "DEBUG: mol->NumAtoms() =" << mol->NumAtoms();
    } else {
        qDebug() << "DEBUG: mol is nullptr!";
    }
    qDebug() << "DEBUG: ehmo pointer =" << ehmo;

    ui.lineEdit_path->setDisabled(true);
    ui.lineEdit_commands->setDisabled(true);
    ui.spinBox_mulplicity->setDisabled(true);

    unsigned numAtoms=mol->NumAtoms();
    qDebug() << "DEBUG: numAtoms =" << numAtoms;
    if (numAtoms<1) {
        qDebug() << "DEBUG: Returning early because numAtoms < 1";
        return;
    }

    charge=ui.spinBox_charge->value();
    mol->setCharge(charge);

    setText("   Extend-Huckel Calculation\n\n");
    appendText("       Coorination of Atoms\n");
    appendText("------------------------------------\n");
    //display atom coordinates
    QString single="";
    char ss[256];
    for(unsigned i=0;i<numAtoms;i++)
    {
        std::sprintf (ss,"%s %10.6f  %10.6f  %10.6f",
                      mol->getAtomSymbol(i).toStdString().c_str(),
                      mol->getAtomXbyIndex(i),
                      mol->getAtomYbyIndex(i),
                      mol->getAtomZbyIndex(i));

        single= QString::fromStdString(ss);
        appendText(single);
        appendText("\n");
    }
    appendText("------------------------------------\n");

    qDebug() << "DEBUG: Calling ehmo->setMol(mol)";
    ehmo->setMol(mol);
    qDebug() << "DEBUG: Calling ehmo->buildBasis()";
    ehmo->buildBasis();

    appendText("\n\n       Basis Sets of Atoms\n");
    appendText("------------------------------------\n");
    appendText("Atom Orbital   IP     Exp    Coeff\n");

    appendText(ehmo->displayBasisSets());
    appendText("------------------------------------\n");
    qDebug() << "DEBUG: initialExtendedHuckel() completed";
}


void HuckelTextForm::runEHMO()
{
    if(ehmo==nullptr) return;

    setWindowTitle("EHMO");

    QApplication::setOverrideCursor(Qt::WaitCursor);
    ehmo->runEHMO();
    QApplication::restoreOverrideCursor();

    //display information
    //vector < double > huckelEigValues;
    //vector < vector < double > > huckelEigVecs;

    QString single="\nOrbital Energy(au):\n";
    QString s;
    for (size_t i=0;i<ehmo->huckelEigValues.size();i++) {
         s = QString("%1   %2\n").arg(i+1, 3).arg(ehmo->huckelEigValues[i], 10, 'f', 4);
         single+=s;
    }
    appendText(single+"\n");

    appendText("\nCalculated MO's coeff:\n");
    for (size_t i=0;i<ehmo->huckelEigVecs.size();i++) {
         single="";
         for (size_t j=0;j<ehmo->huckelEigVecs.size();j++) {
            s = QString("%1").arg(ehmo->huckelEigVecs[i][j], 8, 'f', 4);
            single+=s;
         }
         appendText(single+"\n");
    }

    appendText("\nCalculations fininshed!\n");
    m_parent->updateExtendedHuckelMOs();
}

void HuckelTextForm::factor_eigenMath(Poly & det)
{
    appendText("Factoring polynomial using eigenMath CAS...\n");

    QString polyStr;
    int deg = det.deg();
    for (int i = deg; i >= 0; i--) {
        if (det.c[i] == 0) continue;
        long long c = det.c[i];
        if (!polyStr.isEmpty() && c > 0) polyStr += "+";
        if (i == 0) polyStr += QString::number(c);
        else if (i == 1) polyStr += (c == 1) ? "x" : (c == -1) ? "-x" : QString::number(c) + "x";
        else polyStr += (c == 1) ? QString("x^%1").arg(i)
                     : (c == -1) ? QString("-x^%1").arg(i)
                     : QString::number(c) + QString("x^%1").arg(i);
    }
    if (polyStr.isEmpty()) polyStr = "0";

    char* result = eigenMath::factor_polynomial(polyStr.toUtf8().constData());
    appendText("  factor: " + QString(result) + " = 0\n\n");
}
void HuckelTextForm::factor_lll (Poly & det)
{
#ifdef BIGNUM_HAS_ARBITRARY_PRECISION_FLOAT
    const bool canRunExactLll = BIGNUM_HAS_ARBITRARY_PRECISION_FLOAT;
    appendText(canRunExactLll
                   ? "Factoring polynomial using LLL algorithm...\n"
                   : "Factoring polynomial: exact LLL disabled in no-GMP/no-MPFR build...\n");

    int poly_len = det.deg() + 1;
    mpz_t *poly = (mpz_t*)malloc(poly_len * sizeof(mpz_t));
    mpz_t *factors = (mpz_t*)malloc(poly_len * (poly_len - 1) * sizeof(mpz_t));
    int *multiplicities = (int*)malloc(poly_len * sizeof(int));

    for (int i = 0; i < poly_len; i++) {
        mpz_init(poly[i]);
        mpz_set_si(poly[i], det.c[i]);
    }
    for (int i = 0; i < poly_len * (poly_len - 1); i++) {
        mpz_init(factors[i]);
    }

    int stop_deg = poly_len;
    int factor_counter = 0;
    int usedPrecision = 0;
    double usedDelta = 0.0;

    const bool highDegree = (poly_len >= 16);
    const std::vector<int> quickPrecisions = highDegree
                                                 ? std::vector<int>{128, 192, 256, 384}
                                                 : std::vector<int>{96, 128, 192, 256};
    const std::vector<double> quickDeltas = highDegree
                                                ? std::vector<double>{0.82, 0.90, 0.96}
                                                : std::vector<double>{0.70, 0.82, 0.90};
    const std::vector<int> deepPrecisions = {256, 384, 512, 768};
    const std::vector<double> deepDeltas = {0.90, 0.96, 0.985};
    const qint64 quickBudgetMs = highDegree ? 900 : 1800;
    const qint64 deepBudgetMs = highDegree ? 3500 : 5000;
    const qint64 totalLllBudgetMs = highDegree ? 5000 : 7000;
    QElapsedTimer totalLllTimer;
    totalLllTimer.start();

    auto remainingTotalLllMs = [&]() -> qint64 {
        return totalLllBudgetMs - totalLllTimer.elapsed();
    };

    auto tryLll = [&](mpz_t *p, int len, int stopDeg,
                      mpz_t *factorBuf, int *multBuf,
                      const std::vector<int> &precisions,
                      const std::vector<double> &deltas,
                      qint64 budgetMs,
                      int &bestPrecision, double &bestDelta) -> int {
        if (len <= 1) {
            return 0;
        }

        QElapsedTimer timer;
        timer.start();
        int count = 0;
        for (int precision : precisions) {
            for (double delta : deltas) {
                const qint64 remainingMs = std::min(budgetMs - timer.elapsed(), remainingTotalLllMs());
                if (remainingMs <= 0) {
                    bignum_clear_deadline();
                    return 0;
                }
                for (int i = 0; i < len * (len - 1); ++i) {
                    mpz_set_ui(factorBuf[i], 0);
                }
                for (int i = 0; i < len; ++i) {
                    multBuf[i] = 0;
                }

                bignum_set_deadline_after_ms(remainingMs);
                count = factorize_full(p, len, precision, factorBuf, multBuf, 0, delta, stopDeg);
                bignum_clear_deadline();
                if (count > 0) {
                    bestPrecision = precision;
                    bestDelta = delta;
                    return count;
                }
                if (timer.elapsed() > budgetMs) {
                    return 0;
                }
            }
        }
        return 0;
    };

    auto formatFactors = [&](mpz_t *factorBuf, int *multBuf, int count, int len, int expScale) {
        QString factorStr = "    ";
        for (int j = 0; j < count; j++) {
            QString term = "(";
            mpz_t *f = &factorBuf[j * len];
            bool first = true;
            for (int i = len - 1; i >= 0; i--) {
                if (mpz_sgn(f[i]) != 0) {
                    int exp = i * expScale;
                    if (!first && mpz_sgn(f[i]) > 0) term += "+";
                    if (exp == 0) {
                        term += mpzToQString(f[i]);
                    } else if (exp == 1) {
                        if (mpz_cmp_si(f[i], 1) == 0) term += "x";
                        else if (mpz_cmp_si(f[i], -1) == 0) term += "-x";
                        else {
                            term += mpzToQString(f[i]);
                            term += "x";
                        }
                    } else {
                        if (mpz_cmp_si(f[i], 1) == 0) term += QString("x^%1").arg(exp);
                        else if (mpz_cmp_si(f[i], -1) == 0) term += QString("-x^%1").arg(exp);
                        else {
                            term += mpzToQString(f[i]);
                            term += QString("x^%1").arg(exp);
                        }
                    }
                    first = false;
                }
            }
            term += ")";
            if (multBuf[j] > 1) term += QString("^%1").arg(multBuf[j]);
            factorStr += term + "*";
        }
        if (!factorStr.isEmpty()) factorStr.chop(1);
        return factorStr;
    };

    if (canRunExactLll) {
        factor_counter = tryLll(poly, poly_len, stop_deg,
                                factors, multiplicities,
                                quickPrecisions, quickDeltas, quickBudgetMs,
                                usedPrecision, usedDelta);
    }

    bool usedEvenFallback = false;
    bool usedDeepMode = false;
    if (factor_counter <= 0) {
        bool isEvenPolynomial = true;
        for (int i = 1; i < poly_len; i += 2) {
            if (det.c[i] != 0) {
                isEvenPolynomial = false;
                break;
            }
        }
        if (canRunExactLll && isEvenPolynomial && poly_len >= 3) {
            const int y_len = (poly_len + 1) / 2;
            mpz_t *polyY = (mpz_t*)malloc(y_len * sizeof(mpz_t));
            mpz_t *factorsY = (mpz_t*)malloc(y_len * (y_len - 1) * sizeof(mpz_t));
            int *multY = (int*)malloc(y_len * sizeof(int));

            for (int i = 0; i < y_len; i++) {
                mpz_init(polyY[i]);
                mpz_set_si(polyY[i], det.c[2 * i]);
            }
            for (int i = 0; i < y_len * (y_len - 1); i++) {
                mpz_init(factorsY[i]);
            }

            factor_counter = tryLll(polyY, y_len, y_len,
                                    factorsY, multY,
                                    quickPrecisions, quickDeltas, quickBudgetMs,
                                    usedPrecision, usedDelta);

            if (factor_counter > 0) {
                QString factorStr = formatFactors(factorsY, multY, factor_counter, y_len, 2);
                appendText(factorStr + "=0\n");
                appendText("  [fallback used: factorization in y=x^2]\n");
                usedEvenFallback = true;
            } else {
                factor_counter = tryLll(polyY, y_len, y_len,
                                        factorsY, multY,
                                        deepPrecisions, deepDeltas, deepBudgetMs,
                                        usedPrecision, usedDelta);
                if (factor_counter > 0) {
                    QString factorStr = formatFactors(factorsY, multY, factor_counter, y_len, 2);
                    appendText(factorStr + "=0\n");
                    appendText("  [fallback used: deep factorization in y=x^2]\n");
                    usedEvenFallback = true;
                    usedDeepMode = true;
                }
            }

            for (int i = 0; i < y_len; i++) mpz_clear(polyY[i]);
            for (int i = 0; i < y_len * (y_len - 1); i++) mpz_clear(factorsY[i]);
            free(polyY);
            free(factorsY);
            free(multY);
        }
    }

    if (canRunExactLll && factor_counter <= 0) {
        factor_counter = tryLll(poly, poly_len, stop_deg,
                                factors, multiplicities,
                                deepPrecisions, deepDeltas, deepBudgetMs,
                                usedPrecision, usedDelta);
        if (factor_counter > 0) {
            usedDeepMode = true;
        }
    }

    if (factor_counter > 0)
    {
        if (!usedEvenFallback) {
            QString factorStr = formatFactors(factors, multiplicities, factor_counter, poly_len, 1);
            appendText(factorStr + "=0\n");
        }
        appendText(QString("[LLL settings: precision=%1 bits, delta=%2, mode=%3]\n\n")
                       .arg(usedPrecision)
                       .arg(usedDelta, 0, 'f', 3)
                       .arg(usedDeepMode ? "deep" : "quick"));
    } else {
        if (canRunExactLll) {
            appendText("(LLL did not finish useful exact factors in time; using fast spectral factorization)\n");
        } else {
            appendText("(exact LLL requires arbitrary-precision floating point; using fast spectral factorization)\n");
        }
        if (!hmo->huckelEigValues.empty()) {
            QMap<double, int> eigCount;
            for (const auto& ev : hmo->huckelEigValues) {
                const double x = ev;
                const double rounded = std::round(x * 10000.0) / 10000.0;
                eigCount[rounded]++;
            }
            QString spectral = "    ";
            for (auto it = eigCount.begin(); it != eigCount.end(); ++it) {
                const double r = it.key();
                QString term = "(x";
                if (r > 0.0) term += QString("-%1").arg(r, 0, 'f', 4);
                else if (r < 0.0) term += QString("+%1").arg(-r, 0, 'f', 4);
                term += ")";
                if (it.value() > 1) term += QString("^%1").arg(it.value());
                spectral += term;
            }
            appendText(spectral + "=0\n");
        }
        if (canRunExactLll) {
            appendText(QString("  [LLL time budgets: quick=%1 ms, deep=%2 ms, total=%3 ms]\n\n")
                           .arg(quickBudgetMs)
                           .arg(deepBudgetMs)
                           .arg(totalLllBudgetMs));
        } else {
            appendText("[build mode: local BigInt/BigRat/BigFloat]\n\n");
        }
    }

    for (int i = 0; i < poly_len; i++) mpz_clear(poly[i]);
    for (int i = 0; i < poly_len * (poly_len - 1); i++) mpz_clear(factors[i]);
    free(poly);
    free(factors);
    free(multiplicities);
#else
    (void)det;
#endif
}

void HuckelTextForm::initialHuckel ()
{
    ui.lineEdit_path->setDisabled(true);
    ui.lineEdit_commands->setDisabled(true);
    ui.spinBox_mulplicity->setDisabled(true);

    hmo->setMol(mol);
    charge=ui.spinBox_charge->value();
    hmo->setCharge(charge);
    hmo->buildBasis();

    unsigned int numCarbon=mol->huckelCarbonList.size();

    if (numCarbon<1) return;

    setText("休克尔行列式原子序号:\n");

    QString single=" ";
    for(unsigned i=0;i<numCarbon;i++){
        QString s;
        s = QString(" %1").arg(mol->huckelCarbonList[i]+1, 3);
        single+=s;
        //if (i % 10)  single+="\n";
    }
    appendText(single+"\n");


    for(unsigned i=0;i<numCarbon;i++)
    {
        vector<QString> a (numCarbon,"0");
        a[i]="x";

        for(unsigned j=0;j<numCarbon;j++)
        {
            if(mol->getAtombyIndex(mol->huckelCarbonList[i])->isLinkded(mol->huckelCarbonList[j]))
                a[j]="1";
        }

        //display
        single="|";
        for(unsigned j=0;j<numCarbon;j++) {
            QString s;
            s = QString(" %1").arg(a[j], 3);
            single+=s;
        }
        appendText(single+"   |");
        appendText("\n");
    }

    QChar alpha=QChar(0xb1,0x03);
    QChar beta=QChar(0xb2,0x03);


    //QChar MathSymbolSquereRoot(0x221A);
    //QChar MathSymbolPi(0x03A0);
    //QChar MathSymbolDelta(0x0394);
    //QChar MathSymbolSigma(0x03A3);

    QString str="where x = (";
    str=str+alpha;
    str=str+"-E)/"+beta+"\n";
    appendText(str);

    // Build Hückel polynomial using SimplePoly (simple symbolic expansion)
    appendText("\n\nHückel determinant polynomial:\n");
    
    unsigned n = numCarbon;
    

    // Build adjacency matrix
    std::vector<std::vector<int>> adj(n, std::vector<int>(n, 0));
    for (unsigned i = 0; i < numCarbon; i++) {
        for (unsigned j = 0; j < numCarbon; j++) {
            if (i != j && mol->getAtombyIndex(mol->huckelCarbonList[i])->isLinkded(mol->huckelCarbonList[j])) {
                adj[i][j] = 1;
            }
        }
    }

    // For Hückel: we need det(xI + A) = 0, where A is adjacency matrix (diag 0, off-diag 1 if connected)
    // characteristicPoly computes det(xI - A'), so we pass A' = -A to get det(xI + A)
    std::vector<std::vector<int>> adjNeg(n, std::vector<int>(n, 0));
    for (unsigned i = 0; i < n; i++) {
        for (unsigned j = 0; j < n; j++) {
            adjNeg[i][j] = -adj[i][j];
        }
    }

    // Compute determinant xI + A using Faddeev-LeVerrier algorithm
    Poly det = characteristicPoly(adjNeg, n);

    appendText("  det(H) = " + det.toString() + " = 0\n\n");

    // Factor the polynomial using LLL-based factorization
    if (det.deg() > 0) {
#ifdef BIGNUM_HAS_ARBITRARY_PRECISION_FLOAT
        factor_lll(det);
#else
        factor_eigenMath(det);
#endif
    }

    // Display eigenvalues. HMO::runHuckel stores the Hückel x values directly.
    appendText("Eigenvalues (E = α + xβ):\n");
    if (hmo->huckelEigValues.empty()) {
        appendText(" (Eigenvalues not yet computed - run calculation first)\n");
    } else {
        QMap<double, int> eigCount;
        for (const auto& ev : hmo->huckelEigValues) {
            double x = ev;
            double rounded = std::round(x * 1000.0) / 1000.0;
            eigCount[rounded]++;
        }
        QString eigStr = "  ";
        for (auto it = eigCount.begin(); it != eigCount.end(); ++it) {
            if (it.value() == 1)
                eigStr += QString("x=%1, ").arg(it.key(), 0, 'f', 2);
            else
                eigStr += QString("x=%1 (%2×), ").arg(it.key(), 0, 'f', 2).arg(it.value());
        }
        eigStr.chop(2);
        appendText(eigStr + "\n");
    }

    appendText("\nPlease set the charge of molecule and then run!\n");
}


void HuckelTextForm::on_pushButton_Close_clicked()
{
    close();
}



void HuckelTextForm::appendText(QString s)
{
   s.replace("\n","<br>");
   ui.textEdit->moveCursor(QTextCursor::End);
   ui.textEdit->textCursor().insertHtml("<pre>" + s + "</pre>");
   ui.textEdit->moveCursor(QTextCursor::End);
}



void HuckelTextForm::clear()
{
   ui.textEdit->clear();
}




void HuckelTextForm::setText(QString s)
{
   QString x(s);
   x.replace("\n","<br>");
   ui.textEdit->setText(x);
}



void HuckelTextForm::on_pushButton_run_clicked()
{
    if(isHuckelCal) {
        runHuckel();
        return;
    }

    if(isXTBCal)
        runXTB();

    if(isExtendedHuckelCal) {
        runEHMO();
    }
}




void HuckelTextForm::initialXTB ()
{
    setText("  Perform XTB calculation:\n");
    executePath=m_parent->getExecutePath()+"/tmp";
    appendText("Using "+executePath+" as scratch directory\n");
    QDir dir(executePath);
    if(dir.exists())
        QProcess::execute("rm " + executePath + "/* -rf" );
    else
        QProcess::execute("mkdir " + executePath);


    ui.lineEdit_path->setText(executePath);
    uint numAtoms=mol->NumAtoms();
    QString fileName=executePath+"/test.xyz";

    QFileInfo fileinfo(fileName);
    if (fileinfo.exists()!=false)
        QProcess::execute("rm " + fileName + " -rf");


    commandXTB="xtb " + fileName + " -opt   " ;//+ executePath+"/test.log " ;
    ui.lineEdit_commands->setText(commandXTB);


    //prepare xtb xyz
    QFile file1(fileName);
    if(!file1.open(QIODevice::ReadWrite | QIODevice::Text))
        return;

    QString str2;
    str2 = QString("%1\n").arg(numAtoms, 3);
    file1.write(str2.toUtf8());
    file1.write("run xtb by zhangfq\n");


    double x,y,z;

    appendText("       Coordinates of molecule:\n");
    appendText("----------------------------------------\n");
    for (uint i=0;i<numAtoms;i++)
    {
        QString asymbol=mol->getAtomSymbol(i);

        if (asymbol=="X"|| asymbol=="Bq")       continue;

        x=mol->getAtomXbyIndex(i);
        y=mol->getAtomYbyIndex(i);
        z=mol->getAtomZbyIndex(i);

        str2 = QString("%1 %2  %3  %4\n").arg(asymbol, 3).arg(x, 10, 'f', 5).arg(y, 10, 'f', 5).arg(z, 10, 'f', 5);
        appendText(str2);
        file1.write(str2.toUtf8());
    }
    file1.write("\n\n");
    file1.close();

    appendText("----------------------------------------\n");
    appendText("\n");
}



void HuckelTextForm::runXTB()
{
    setWindowTitle("XTB");
    if (runShellProcess==nullptr)
        runShellProcess=new QProcess(this);

    if(runShellProcess) {
        appendText("New process ok!\n");
    }

    runShellProcess->start(commandXTB);

    log="";
    connect(runShellProcess, SIGNAL(readyRead()), this, SLOT(readProcess()));
    connect(runShellProcess, SIGNAL(finished(int)), this, SLOT(finishedProcess()));
}



void HuckelTextForm::readProcess()
{
    log = QString::fromLocal8Bit(runShellProcess->readAll());
    appendText(log);
}


void HuckelTextForm::finishedProcess()
{
    if (runShellProcess) {
        disconnect(runShellProcess, SIGNAL(readyRead()), this, SLOT(readProcess()));
        disconnect(runShellProcess, SIGNAL(finished(int)), this, SLOT(finishedProcess()));
    }

    //mol->loadFile(m_parent->getExecutePath()+"/xtbopt.xyz");
    QString c=" cat "+m_parent->getExecutePath()+"/xtbopt.xyz";
    runShellProcess->start(c);
    runShellProcess->waitForFinished();
    QByteArray qByteRead = runShellProcess->readAllStandardOutput();
    appendText("\n   Optimzied Coordinate\n");
    QString result=QString::fromLocal8Bit(qByteRead);
    appendText(result);


    //refresh molecule data
    mol->loadxyzfromQString(result);
    //mol->centeralize();
    view3d->renderMol(mol);
}



void HuckelTextForm::runHuckel()
{
    setWindowTitle("Huckel");

    QApplication::setOverrideCursor(Qt::WaitCursor);
    hmo->runHuckel();
    QApplication::restoreOverrideCursor();


    QChar  alpha=QChar(0xb1,0x03);
    QChar  beta=QChar(0xb2,0x03);

    appendText("\nSolutions of this polynominals: ");
    QString str=" where x = (";
    str=str+alpha;
    str=str+"-E)/"+beta+"\n";
    appendText(str);

    QString single="x=";
    for (uint i=0;i<hmo->huckelEigValues.size();i++) {
        QString s;
        s = QString(" %1").arg(hmo->huckelEigValues[i], 10, 'f', 4);
        single+=s;
    }
    appendText(single+"\n");


    uint size=hmo->huckelEigValues.size();
    for (uint i=0;i<hmo->huckelEigValues.size();i++)
    {
        single="E"+QString::number(size)+" =";
        QString s;
        if (fabs(hmo->huckelEigValues[i])>0.0001){
            s = QString("+%1").arg(hmo->huckelEigValues[i], 6, 'f', 4);
            single += alpha+s+beta;
        }
        else
            single += alpha;

        single.remove(RE_WHITESPACE);
        appendText(single+"\n");
        size--;
    }

    //https://www.utf8-chartable.de/unicode-utf8-table.pl?start=768
    QString  Psi= QString::fromUtf8("\u03A8");
    QString  phi= QString::fromUtf8("\u03C6");


    appendText("\n");
    QChar pair=QChar(0x21F5);
    QChar spinUp=QChar(0x2191);
    QString str_;
    int numElectrons;
    size=hmo->huckelEigValues.size();

    for (uint i=0;i<hmo->huckelEigVecs.size();i++)
    {
        double d=hmo->huckelEigValues[i];
        double dn=hmo->huckelEigValues[i+1];
        double dp=0.0;

        numElectrons=hmo->numElectrons()-2*(hmo->huckelEigVecs.size()-1-i);

        if(abs(d-dn)<0.01 && numElectrons==0 )
            numElectrons=1;
        if(i>0 ){
            dp=hmo->huckelEigValues[i-1];
            if(abs(d-dp)<0.01 && numElectrons==2)
            numElectrons=1;
        }
        if(numElectrons<0)
            numElectrons=0;

        str_="(";
        if(numElectrons==0) {
            str_ += "-";
        }
        if(numElectrons>1) {
            str_ += pair;
        }
        if(numElectrons==1) {
            str_ += spinUp;
        }
        str_+=")";

        single =str_;
        single +=Psi+QString::number(size)+"=";
        for (uint j=0;j<hmo->huckelEigVecs[i].size();j++) {
            QString s;
            if(hmo->huckelEigVecs[i][j]>-0.00000001)
                s = QString(" +%1").arg(hmo->huckelEigVecs[i][j], 6, 'f', 4);
            else
                s = QString(" %1").arg(hmo->huckelEigVecs[i][j], 6, 'f', 4);
            single+=s+phi+QString::number(hmo->huckelCarbonList[j]+1);
        }
        single.remove(QRegularExpression("\\s"));
        appendText(single+"\n");
        size--;    }

    appendText("\nCalculations fininshed!\n");
    m_parent->updateHuckelMOs();

    // output += QString::fromLocal8Bit(pProces->readAll());
}


void HuckelTextForm::on_spinBox_charge_valueChanged(int arg1)
{
    charge=arg1;
}


void HuckelTextForm::on_spinBox_mulplicity_valueChanged(int arg1)
{
    multipilicty=arg1;
}


void HuckelTextForm::on_lineEdit_path_textChanged(const QString &arg1)
{
    executePath=ui.lineEdit_path->text();
}
