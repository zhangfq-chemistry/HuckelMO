QT     += core gui widgets charts

requires(qtConfig(filedialog))
requires(qtConfig(treeview))


DEFINES+=DMS_INCLUDE_SOURCE

MKLDIR = $$(MKLROOT)


LIBS += -L/usr/local/lib \
    -L/opt/Qt/6.10.1/gcc_64/lib \
    -L/home/zhangfq/.local/lib \
    -lQtColorWidgets \
    -lopenbabel \
    -lgmp \
    -lmpfr \
    -lmpc \


INCLUDEPATH += . \
    /home/zhangfq/sxnu/QtColorWidgets \
    math/newMatrix \
    /usr/local/include/vtk-9.6 \
    /usr/local/include/openbabel3 \
    /usr/local/include \
    /home/zhangfq/.local/include


DEPENDPATH  += . \
    math/newMatrix \
    /usr/local/include/vtk-9.6 \
    /usr/local/include/openbabel3 \
    /usr/local/include \
    /home/zhangfq/.local/include





CONFIG -= gnu++11
CONFIG += c++20

QMAKE_CXXFLAGS = -std=c++20 -Wall -Wextra -fopenmp
QMAKE_CXXFLAGS_RELEASE = -std=c++20 -O3 -march=native -ffast-math  -Wall -Wextra -fopenmp
QMAKE_LFLAGS_RELEASE += -O3 -fopenmp


CONFIG(win32, win32|macx){
    VTK_INCLUDE_DIR = D:\VTK\vtk\include
    VTK_LIB_DIR = D:\VTK\vtkOut\lib
    VTK_LIB_VER = 8.90
    VTK_LIB_PREFIX = $${VTK_LIB_VER}
    VTK_LIB_NAME =  vtkalglib\
                    vtkChartsCore\
                    vtkCommonColor\
                    vtkCommonComputationalGeometry\
                    vtkCommonCore\
                    vtkCommonDataModel\
                    vtkCommonExecutionModel\
                    vtkCommonMath\
                    vtkCommonMisc\
                    vtkCommonSystem\
                    vtkCommonTransforms\
                    vtkDICOMParser\
                    vtkDomainsChemistry\
                    vtkDomainsChemistryOpenGL2\
                    vtkFiltersAMR\
                    vtkFiltersCore\
                    vtkFiltersExtraction\
                    vtkFiltersFlowPaths\
                    vtkFiltersGeneral\
                    vtkFiltersGeneric\
                    vtkFiltersGeometry\
                    vtkFiltersHybrid\
                    vtkFiltersHyperTree\
                    vtkFiltersImaging\
                    vtkFiltersModeling\
                    vtkFiltersParallel\
                    vtkFiltersParallelImaging\
                    vtkFiltersPoints\
                    vtkFiltersProgrammable\
                    vtkFiltersSelection\
                    vtkFiltersSMP\
                    vtkFiltersSources\
                    vtkFiltersStatistics\
                    vtkFiltersTexture\
                    vtkFiltersTopology\
                    vtkFiltersVerdict\
                    vtkfreetype\
                    vtkGeovisCore\
                    #vtkgl2ps\
                    #vtkglew\
                    vtkhdf5_hl\
                    vtkhdf5\
                    vtkImagingColor\
                    vtkImagingCore\
                    vtkImagingFourier\
                    vtkImagingGeneral\
                    vtkImagingHybrid\
                    vtkImagingMath\
                    vtkImagingMorphological\
                    vtkImagingSources\
                    vtkImagingStatistics\
                    vtkImagingStencil\
                    vtkInfovisCore\
                    vtkInfovisLayout\
                    vtkInteractionImage\
                    vtkInteractionStyle\
                    vtkInteractionWidgets\
                    vtkIOAMR\
                    vtkIOCore\
                    vtkIOEnSight\
                    vtkIOExodus\
                    vtkIOExport\
                    #vtkIOExportOpenGL2\
                    vtkIOGeometry\
                    vtkIOImage\
                    vtkIOImport\
                    vtkIOInfovis\
                    vtkIOLegacy\
                    vtkIOLSDyna\
                    vtkIOMINC\
                    vtkIOMovie\
                    vtkIONetCDF\
                    vtkIOParallel\
                    vtkIOParallelXML\
                    vtkIOPLY\
                    vtkIOSQL\
                    vtkIOTecplotTable\
                    vtkIOVideo\
                    vtkIOXML\
                    vtkIOXMLParser\
                    vtkjpeg\
                    vtkjsoncpp\
                    vtklibharu\
                    vtklibxml2\
                    vtklz4\
                    vtkmetaio\
                    #vtkNetCDF\
                    vtkParallelCore\
                    vtkpng\
                  # vtknetcdfcpp\
                   # vtkoggtheora\
                   # vtkproj4\
                 #   vtkexoIIc\
               #     vtkexpat\
                    vtkRenderingAnnotation\
                    vtkRenderingContext2D\
                    vtkRenderingContextOpenGL2\
                    vtkRenderingCore\
                    vtkRenderingFreeType\
                    vtkRenderingGL2PSOpenGL2\
                    vtkRenderingImage\
                    vtkRenderingLabel\
                    vtkRenderingLOD\
                    vtkRenderingOpenGL2\
                    vtkRenderingVolume\
                    vtkRenderingVolumeOpenGL2\
                    vtksqlite\
                    vtksys\
                    vtktiff\
                    vtkverdict\
                    vtkViewsContext2D\
                    vtkViewsCore\
                    vtkViewsInfovis\
                    vtkzlib
    for(lib,VTK_LIB_NAME){
        VTK_LIBS += -l$${lib}-$${VTK_LIB_PREFIX}
    }
    INCLUDEPATH += $${VTK_INCLUDE_DIR}
    DEPENDPATH += $${VTK_INCLUDE_DIR}
    LIBS += -L$${VTK_LIB_DIR} $${VTK_LIBS}
}
else{ #LINUX
#VTK 9.6 configuration
VTK_INCLUDE_DIR = /usr/local/include/vtk-9.6
VTK_LIB_DIR = /usr/local/lib
VTK_LIB_VER = 9.6
VTK_LIB_PREFIX = $${VTK_LIB_VER}
    VTK_LIB_NAME =  vtkChartsCore\
                    vtkCommonColor\
                    vtkCommonComputationalGeometry\
                    vtkCommonCore\
                    vtkCommonDataModel\
                    vtkCommonExecutionModel\
                    vtkCommonMath\
                    vtkCommonMisc\
                    vtkCommonSystem\
                    vtkCommonTransforms\
                    vtkDICOMParser\
                    vtkDomainsChemistry\
                    vtkDomainsChemistryOpenGL2\
                    vtkFiltersAMR\
                    vtkFiltersCore\
                    vtkFiltersExtraction\
                    vtkFiltersFlowPaths\
                    vtkFiltersGeneral\
                    vtkFiltersGeneric\
                    vtkFiltersGeometry\
                    vtkFiltersHybrid\
                    vtkFiltersHyperTree\
                    vtkFiltersImaging\
                    vtkFiltersModeling\
                    vtkFiltersParallel\
                    vtkFiltersParallelImaging\
                    vtkFiltersPoints\
                    vtkFiltersProgrammable\
                    vtkFiltersSMP\
                    vtkFiltersSelection\
                    vtkFiltersSources\
                    vtkFiltersStatistics\
                    vtkFiltersTexture\
                    vtkFiltersTopology\
                    vtkFiltersVerdict\
                    vtkGUISupportQt\
                    vtkGUISupportQtSQL\
                    vtkGeovisCore\
                    vtkIOAMR\
                    vtkIOCore\
                    vtkIOEnSight\
                    vtkIOExodus\
                    vtkIOExport\
                    #vtkIOExportOpenGL2\
                    vtkIOGeometry\
                    vtkIOImage\
                    vtkIOImport\
                    vtkIOInfovis\
                    vtkIOLSDyna\
                    vtkIOLegacy\
                    vtkIOMINC\
                    vtkIOMovie\
                    vtkIONetCDF\
                    vtkIOPLY\
                    vtkIOParallel\
                    vtkIOParallelXML\
                    vtkIOSQL\
                    vtkIOTecplotTable\
                    vtkIOVideo\
                    vtkIOXML\
                    vtkIOXMLParser\
                    vtkImagingColor\
                    vtkImagingCore\
                    vtkImagingFourier\
                    vtkImagingGeneral\
                    vtkImagingHybrid\
                    vtkImagingMath\
                    vtkImagingMorphological\
                    vtkImagingSources\
                    vtkImagingStatistics\
                    vtkImagingStencil\
                    vtkInfovisCore\
                    vtkInfovisLayout\
                    vtkInteractionImage\
                    vtkInteractionStyle\
                    vtkInteractionWidgets\
                    #vtkNetCDF\
                    vtkParallelCore\
                    vtkRenderingAnnotation\
                    vtkRenderingContext2D\
                    vtkRenderingContextOpenGL2\
                    vtkRenderingCore\
                    vtkRenderingFreeType\
                    vtkRenderingGL2PSOpenGL2\
                    vtkRenderingImage\
                    vtkRenderingLOD\
                    vtkRenderingLabel\
                    vtkRenderingOpenGL2\
                    vtkRenderingQt\
                    vtkRenderingVolume\
                    vtkRenderingVolumeOpenGL2\
                    vtkViewsContext2D\
                    vtkViewsCore\
                    vtkViewsInfovis\
                    vtkViewsQt\
                    vtkexpat\
                    vtkfreetype\
                    vtkgl2ps\
                    #vtkglew\
                    vtkhdf5\
                    vtkhdf5_hl\
                    vtkjpeg\
                    vtkjsoncpp\
                    vtklibharu\
                    vtklibxml2\
                    vtklz4\
                    vtkmetaio\
                   # vtknetcdfcpp\
                   # vtkoggtheora\
                   # vtkalglib\
                   # vtkexoIIc\
                   # vtkproj4\
                   # vtkLocalExample\
                    vtkpng\
                    vtksqlite\
                    vtksys\
                    vtktiff\
                    vtkverdict\
                    vtkzlib
    for(lib,VTK_LIB_NAME){
        VTK_LIBS += -l$${lib}-$${VTK_LIB_PREFIX}
    }
    INCLUDEPATH += $${VTK_INCLUDE_DIR}
    DEPENDPATH += $${VTK_INCLUDE_DIR}
    LIBS += -L$${VTK_LIB_DIR} $${VTK_LIBS}
}





HEADERS       += mainwindow.h \
    Mol.h \
    aboutDialog.h \
    huckelTextForm.h \
    math/symmetry.h \
    math/vector3.h \
    math/matrix3x3.h \
    textEditor.h \
    View3D.h \
    utils.h \
    bondProperty.h \
    templateMol.h \
    templateView3d.h \
    rTemplate.h \
    atomTemplate.h \
    angleProperty.h \
    torsionProperty.h \
    ringTemplate.h \
    periodicTable.h \
    eigenMath.h \
    vtkbool/AABB.h \
    vtkbool/Decomposer.h \
    vtkbool/Merger.h \
    vtkbool/RmTrivials.h \
    vtkbool/Tools.h \
    vtkbool/Utilities.h \
    vtkbool/VisPoly.h \
    vtkbool/vtkPolyDataBooleanFilter.h \
    vtkbool/vtkPolyDataContactFilter.h \
    AreaPicker.h \
    codeEditor.h \
    atomProperty.h \
    ringProperty.h \
    orbitalProperty.h \
    InteractorTView3d.h \
    InteractorView3d.h \
    vtkBestFitPlane.h \
    EHMO.h \
    functionals.h \
    HMO.h \
    polyfactor/lll_gs.h \
    polyfactor/lll_functions.h \



SOURCES       += main.cpp \
    Mol.cpp \
    aboutDialog.cpp \
    huckelTextForm.cpp \
    mainwindow.cpp \
    math/matrix3x3.cpp \
    math/symmetry.cpp \
    math/vector3.cpp \
    textEditor.cpp \
    View3D.cpp \
    utils.cpp \
    bondProperty.cpp \
    templateMol.cpp \
    templateView3d.cpp \
    rTemplate.cpp \
    atomTemplate.cpp \
    angleProperty.cpp \
    torsionProperty.cpp \
    ringTemplate.cpp \
    periodicTable.cpp \
    eigenMath.cpp \
    vtkbool/Decomposer.cpp \
    vtkbool/Merger.cpp \
    vtkbool/PyInit.cxx \
    vtkbool/RmTrivials.cpp \
    vtkbool/Tools.cpp \
    vtkbool/Utilities.cpp \
    vtkbool/VisPoly.cpp \
    vtkbool/vtkPolyDataBooleanFilter.cpp \
    vtkbool/vtkPolyDataContactFilter.cpp \
    AreaPicker.cpp \
    codeEditor.cpp \
    atomProperty.cpp \
    ringProperty.cpp \
    orbitalProperty.cpp \
    InteractorTView3d.cpp \
    InteractorView3d.cpp \
    vtkBestFitPlane.cxx \
    EHMO.cpp \
    functionals.cpp \
    HMO.cpp \



#https://github.com/geodavic/poly_factor.git


FORMS += \
    UI/about.ui \
    UI/mainwindow.ui \
    UI/angle.ui \
    UI/bond.ui \
    UI/torsion.ui \
    UI/ringTemplate.ui \
    UI/periodicTable.ui \
    UI/AtomTemplate.ui \
    UI/dialogAtom.ui \
    UI/dialogRing.ui \
    UI/huckelTextForm.ui \
    UI/MOProperty.ui




RESOURCES +=  hmo.qrc


