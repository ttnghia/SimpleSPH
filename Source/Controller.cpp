//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
//  Copyright (c) 2017 by
//       __      _     _         _____
//    /\ \ \__ _| |__ (_) __ _  /__   \_ __ _   _  ___  _ __   __ _
//   /  \/ / _` | '_ \| |/ _` |   / /\/ '__| | | |/ _ \| '_ \ / _` |
//  / /\  / (_| | | | | | (_| |  / /  | |  | |_| | (_) | | | | (_| |
//  \_\ \/ \__, |_| |_|_|\__,_|  \/   |_|   \__,_|\___/|_| |_|\__, |
//         |___/                                              |___/
//
//  <nghiatruong.vn@gmail.com>
//  All rights reserved.
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

#include "Common.h"
#include "Controller.h"

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void Controller::setupGUI()
{
    QVBoxLayout* controlLayout = new QVBoxLayout;
    QVBoxLayout* btnLayout     = new QVBoxLayout;

    setupTextureControllers(controlLayout);
    setupColorControllers(controlLayout);
    setupSimulationControllers(controlLayout);

    m_LightEditor = new PointLightEditor(nullptr, this);
    QWidget* mainControls = new QWidget;
    mainControls->setLayout(controlLayout);

    QTabWidget* tabWidget = new QTabWidget;
    tabWidget->setTabPosition(QTabWidget::South);
    tabWidget->setTabShape(QTabWidget::Triangular);
    tabWidget->addTab(mainControls,  "Main Params");
    tabWidget->addTab(m_LightEditor, "Lights");

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);
    mainLayout->addStretch();
    mainLayout->addLayout(btnLayout);
    setLayout(mainLayout);

    ////////////////////////////////////////////////////////////////////////////////
    setFixedWidth(300);
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void Controller::updateSimParams(const std::shared_ptr<SimulationParameters>& simParams)
{
    simParams->numThreads        = m_cbNumThreads->currentIndex();
    simParams->pressureStiffness = m_txtPressureStiffness->text().toFloat();
    simParams->viscosity         = m_txtViscosity->text().toFloat();
    simParams->stopTime          = m_txtStopTime->text().toFloat();
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void Controller::disableParameters(bool disable)
{
    m_cbNumThreads->setDisabled(disable);
    m_cbSimulationScene->setDisabled(disable);
    m_txtPressureStiffness->setDisabled(disable);
    m_txtViscosity->setDisabled(disable);
    m_txtStopTime->setDisabled(disable);
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void Controller::loadTextures()
{
    ////////////////////////////////////////////////////////////////////////////////
    // sky textures
    int currentSkyTexID = m_cbSkyTexture->getComboBox()->currentIndex();
    m_cbSkyTexture->getComboBox()->clear();
    m_cbSkyTexture->getComboBox()->addItem("None");
    QStringList skyTexFolders = getTextureFolders("Sky");

    foreach(QString tex, skyTexFolders)
    {
        m_cbSkyTexture->getComboBox()->addItem(tex);
    }

    m_cbSkyTexture->getComboBox()->setCurrentIndex(currentSkyTexID > 0 ? currentSkyTexID : 0);

    ////////////////////////////////////////////////////////////////////////////////
    // floor textures
    int currentFloorTexID = m_cbFloorTexture->getComboBox()->currentIndex();
    m_cbFloorTexture->getComboBox()->clear();
    QStringList floorTexFolders = getTextureFiles("Floor");
    m_cbFloorTexture->getComboBox()->addItem("None");

    foreach(QString tex, floorTexFolders)
    {
        m_cbFloorTexture->getComboBox()->addItem(tex);
    }

    m_cbFloorTexture->getComboBox()->setCurrentIndex(currentFloorTexID > 0 ? currentFloorTexID : 0);
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void Controller::setupSimulationControllers(QVBoxLayout* ctrLayout)
{
    ////////////////////////////////////////////////////////////////////////////////
    // simulation controllers
    m_cbNumThreads = new QComboBox;
    m_cbNumThreads->addItem(QString("Automatic"));

    for(int i = 1; i <= 64; ++i)
        m_cbNumThreads->addItem(QString("%1").arg(i));

    m_cbSimulationScene = new QComboBox;
    m_cbSimulationScene->addItems(SimulationSceneNames);

    m_txtPressureStiffness = new QLineEdit;
    m_txtPressureStiffness->setText(QString("%1").arg(DEFAULT_PRESSURE_STIFFNESS));

    m_txtViscosity = new QLineEdit;
    m_txtViscosity->setText(QString("%1").arg(DEFAULT_VISCOSITY));

    m_txtStopTime = new QLineEdit;
    m_txtStopTime->setText("5000");

    ////////////////////////////////////////////////////////////////////////////////
    QGridLayout* simControllerLayout = new QGridLayout;
    simControllerLayout->setContentsMargins(2, 2, 2, 2);
    simControllerLayout->addWidget(new QLabel("Num. Threads: "),       0, 0, 1, 1);
    simControllerLayout->addWidget(m_cbNumThreads,                     0, 1, 1, 2);

    simControllerLayout->addWidget(new QLabel("Scene: "),              1, 0, 1, 1);
    simControllerLayout->addWidget(m_cbSimulationScene,                1, 1, 1, 2);

    simControllerLayout->addWidget(new QLabel("Pressure Stiffness: "), 2, 0, 1, 1);
    simControllerLayout->addWidget(m_txtPressureStiffness,             2, 1, 1, 2);

    simControllerLayout->addWidget(new QLabel("Viscosity: "),          3, 0, 1, 1);
    simControllerLayout->addWidget(m_txtViscosity,                     3, 1, 1, 2);

    simControllerLayout->addWidget(new QLabel("Stop time(ms): "),      4, 0, 1, 1);
    simControllerLayout->addWidget(m_txtStopTime,                      4, 1, 1, 2);


    QGroupBox* grpSimControllers = new QGroupBox;
    grpSimControllers->setTitle("Simulation Parameters");
    grpSimControllers->setLayout(simControllerLayout);

    ////////////////////////////////////////////////////////////////////////////////
    m_btnStartStopSimulation = new QPushButton("Start");
    m_btnResetSimulation     = new QPushButton("Reset");

    QGridLayout* btnSimControlLayout = new QGridLayout;
    btnSimControlLayout->addWidget(m_btnStartStopSimulation, 0, 0, 1, 1);
    btnSimControlLayout->addWidget(m_btnResetSimulation,     0, 1, 1, 1);

    ////////////////////////////////////////////////////////////////////////////////
    ctrLayout->addWidget(grpSimControllers);
    ctrLayout->addStretch();
    ctrLayout->addLayout(btnSimControlLayout);
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void Controller::setupTextureControllers(QBoxLayout* ctrLayout)
{
    ////////////////////////////////////////////////////////////////////////////////
    // sky textures
    m_cbSkyTexture = new EnhancedComboBox;
    ctrLayout->addWidget(m_cbSkyTexture->getGroupBox("Sky Texture"));

    ////////////////////////////////////////////////////////////////////////////////
    // floor textures
    m_cbFloorTexture = new EnhancedComboBox;

    m_sldFloorSize = new EnhancedSlider;
    m_sldFloorSize->setRange(1, 100);
    m_sldFloorSize->getSlider()->setValue(DEFAULT_FLOOR_SIZE);

    m_sldFloorExposure = new EnhancedSlider;
    m_sldFloorExposure->setRange(1, 100);
    m_sldFloorExposure->getSlider()->setValue(100);

    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    QVBoxLayout* floorTexLayout = new QVBoxLayout;
    floorTexLayout->addLayout(m_cbFloorTexture->getLayout());
    floorTexLayout->addSpacing(10);
    floorTexLayout->addWidget(line);
    floorTexLayout->addLayout(m_sldFloorSize->getLayoutWithLabel("Size:"));
    floorTexLayout->addLayout(m_sldFloorExposure->getLayoutWithLabel("Exposure:"));

    QGroupBox* floorTexGroup = new QGroupBox;
    floorTexGroup->setTitle("Floor Texture");
    floorTexGroup->setLayout(floorTexLayout);
    ctrLayout->addWidget(floorTexGroup);

    ////////////////////////////////////////////////////////////////////////////////
    loadTextures();
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void Controller::setupColorControllers(QBoxLayout* ctrLayout)
{
    ////////////////////////////////////////////////////////////////////////////////
    /// color modes
    QRadioButton* rdbColorRandom  = new QRadioButton("Random");
    QRadioButton* rdbColorRamp    = new QRadioButton("Ramp");
    QRadioButton* rdbColorUniform = new QRadioButton("Uniform");

    rdbColorRandom->setChecked(true);

    QGridLayout* colorModeLayout = new QGridLayout;
    colorModeLayout->addWidget(rdbColorRandom,  0, 0);
    colorModeLayout->addWidget(rdbColorRamp,    0, 1);
    colorModeLayout->addWidget(rdbColorUniform, 1, 0);

    QGroupBox* colorModeGroup = new QGroupBox;
    colorModeGroup->setTitle(tr("Particle Color Mode"));
    colorModeGroup->setLayout(colorModeLayout);
    ctrLayout->addWidget(colorModeGroup);

    m_smParticleColorMode = new QSignalMapper(this);
    connect(rdbColorRandom,  SIGNAL(clicked()), m_smParticleColorMode, SLOT(map()));
    connect(rdbColorRamp,    SIGNAL(clicked()), m_smParticleColorMode, SLOT(map()));
    connect(rdbColorUniform, SIGNAL(clicked()), m_smParticleColorMode, SLOT(map()));

    m_smParticleColorMode->setMapping(rdbColorRandom,  static_cast<int>(ParticleColorMode::Random));
    m_smParticleColorMode->setMapping(rdbColorRamp,    static_cast<int>(ParticleColorMode::Ramp));
    m_smParticleColorMode->setMapping(rdbColorUniform, static_cast<int>(ParticleColorMode::Uniform));

    m_msParticleMaterial = new MaterialSelector;
    m_msParticleMaterial->setCustomMaterial(CUSTOM_PARTICLE_MATERIAL);
    m_msParticleMaterial->setDefaultCustomMaterial(true);

    QGridLayout* particleColorLayout = new QGridLayout;
//    particleColorLayout->addWidget(new QLabel("Sphere View: "), 0, 0, Qt::AlignRight);
    particleColorLayout->addLayout(m_msParticleMaterial->getLayout(), 0, 1, 1, 2);

    QGroupBox* particleColorGroup = new QGroupBox("Uniform Color");
    particleColorGroup->setLayout(particleColorLayout);
    ctrLayout->addWidget(particleColorGroup);
}
