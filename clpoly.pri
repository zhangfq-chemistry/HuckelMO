# Local CLPoly sources
CLPOLY_PATH = $$PWD

INCLUDEPATH += $$CLPOLY_PATH
DEPENDPATH += $$CLPOLY_PATH

HEADERS += \
    $$CLPOLY_PATH/clpoly.h \
    $$CLPOLY_PATH/basic.h \
    $$CLPOLY_PATH/basic_monomial.h \
    $$CLPOLY_PATH/basic_polynomial.h \
    $$CLPOLY_PATH/charset.h \
    $$CLPOLY_PATH/dense_upoly_zp.h \
    $$CLPOLY_PATH/groebner.h \
    $$CLPOLY_PATH/monomial.h \
    $$CLPOLY_PATH/monomial_order.h \
    $$CLPOLY_PATH/number.h \
    $$CLPOLY_PATH/ZZ.h \
    $$CLPOLY_PATH/QQ.h \
    $$CLPOLY_PATH/basic.h \
    $$CLPOLY_PATH/polynomial_convert.h \
    $$CLPOLY_PATH/polynomial_factorize.h \
    $$CLPOLY_PATH/polynomial_factorize_univar.h \
    $$CLPOLY_PATH/polynomial_factorize_wang.h \
    $$CLPOLY_PATH/polynomial_factorize_zp.h \
    $$CLPOLY_PATH/polynomial_gcd.h \
    $$CLPOLY_PATH/polynomial.h \
    $$CLPOLY_PATH/polynomial_.h \
    $$CLPOLY_PATH/polynomial_type.h \
    $$CLPOLY_PATH/random.h \
    $$CLPOLY_PATH/realroot.h \
    $$CLPOLY_PATH/resultant.h \
    $$CLPOLY_PATH/upolynomial.h \
    $$CLPOLY_PATH/variable.h \

SOURCES += \
    $$CLPOLY_PATH/variable.cc \
    $$CLPOLY_PATH/upolynomial.cc \
    $$CLPOLY_PATH/polynomial_gcd.cc \
    $$CLPOLY_PATH/realroot.cc \
    $$CLPOLY_PATH/associatedgraph.cc \


