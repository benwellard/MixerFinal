#include "mixer.h"
#include <QWidget>
#include <volslider.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <volumelevel.h>
#include <QLabel>
#include <QString>
#include <QPushButton>
#include <mixer_main.h>
#include <QComboBox>
#include <interface.h>
#include <iostream>
#include <usb_mixer.h>
#include <controller.h>
#include <QScrollArea>
#include <channelbox.h>
#include <QGroupBox>
#include <QCheckBox>
#include <outputbox.h>
#include <qfingerscrollarea.h>
#include <masterslider.h>

int noOfChannelsMixer = 0;

Mixer::Mixer(int output, int mixerOut, Controller *controller, int mixRef) : QWidget()
{
    //Creates interface object for controller-mixer operations
    interface = new Interface(output, mixerOut, controller, mixRef);
    mixChannel = output;
    mixReference = mixRef;

    //Controller pointer
    controllerp = controller;
    numIns = usb_mixer_get_num_inputs(0);
    //Does nothing
    constructMixer(output);


}

void Mixer::setChannels(int n)
{
    emit emitChannels(noOfChannelsMixer);
}

void Mixer::addChannel()
{
    interface->addChannel(selectedChannel);
    noOfChannelsMixer++;
    setChannels(noOfChannelsMixer);
}

void Mixer::newSlider()
{
    //Create the slider object, passing the current index and whatever channel it is assigned to
    VolSlider *mySlider = new VolSlider(indexNo, selectedChannel);

    //Add the slider to an array(QList)
    sliders.insert(indexNo, mySlider);

    //Retrieves the slider's relevant channel's current volume level and sets that to the slider
    sliders[indexNo]->setInitValue(interface->getValue(sliders[indexNo]->channel));

    //Sets slider size
    sliders[indexNo]->setMinimumSize(100,250);
    sliders[indexNo]->setMaximumSize(100,300);

    //Connects the change in the slider' value with a function to release that value along with current index
    connect(sliders[indexNo],SIGNAL(valueChanged(int)),sliders[indexNo],SLOT(releaseIndex(int)));

    //Connects a slider to this object, emitting current volume level and index
    connect(sliders[indexNo],SIGNAL(emitIndex(int, int)),this,SLOT(changeVolume(int,int)));

    //Slider layout adjustment
    sliderLayout->setSpacing(10);
    QGroupBox *sliderBox = new QGroupBox();
    sliderBox->setStyleSheet("background-color: white;");
    sliderBox->setFixedWidth(170);
    QHBoxLayout *sliderBoxLayout = new QHBoxLayout();
    sliderBox->setLayout(sliderBoxLayout);
    sliderBoxLayout->setAlignment(this,Qt::AlignRight);
    sliderBoxLayout->addStretch(50000);
    sliderBoxes.insert(indexNo,sliderBox);
    sliderBoxLayout->addWidget(sliders[indexNo]);
    sliderLayout->addWidget(sliderBox,Qt::AlignLeft);
    sliderLayout->addStretch(5);


    //##########################START OF CHANNELBOX CODE ################################################################
    //Create the channel selector box
    ChannelBox *channelBox = new ChannelBox(indexNo);
    channelBox->setStyleSheet("QComboBox QAbstractItemView { background-color: white; selection-color: red; border: 2px solid red;} QComboBox { background-color: white; border: 2px solid black;}");
    //Add the channel selector to an array (Qlist)
    selectors.insert(indexNo, channelBox);

    //Set the selector's size
    channelBox->setFixedSize(60,50);

    //Loop that fills the selector box with channels taken from the device
    for(int i = 0;i < numIns;i++)
    {
        //Creates a string with data pulled from device
        QString myString = usb_mixsel_get_input_string(0,i);

        //Trim the string to remove DAW/AUD
        myString.remove(1,2);
        myString.remove(5,myString.length()-6);

        //Add string to box
        channelBox->addItem(myString,Qt::DisplayRole);


    }
    //Sets the selector to whatever the slider's current channel is
    //Read from device, find out name of whatever channel is currently at usb_mixer_get_input_name(0, indexNo)
    //That channel is what this channel is currently assigned to.
    QString *channelName = new QString(usb_mixsel_get_input_string(mixReference,indexNo));              //(us(0,indexNo));

    int curInd;

    curInd = controllerp->getMixerMap(indexNo,mixReference);
    channelBox->setCurrentIndex(curInd);
    channelBox->channel = curInd;
    interface->updateChannelMap(curInd);
    //Connects the channel selector to the relevant slider, adjusting what channel that slider is currently operating
    //connect(channelBox,SIGNAL(activated(int)),sliders[indexNo],SLOT(changeChannel(int)));

    connect(selectors[indexNo],SIGNAL(activated(int)),selectors[indexNo],SLOT(setChannel(int)));
    connect(selectors[indexNo],SIGNAL(emitChannel(int,int)),interface,SLOT(setChannel(int,int)));


    //#################END OF CHANNELBOX CODE ##########################################################################
    
    
    
    //Create the mute button layout and button, and set size and text
    QVBoxLayout *muteChannelLayout = new QVBoxLayout();
    QPushButton *muteButton = new QPushButton();
    muteButton->setStyleSheet("QPushButton { image: url(:/new/prefix1/muteimage.jpg); border: 5px solid black; border-radius:5px;}");
    mutes.insert(indexNo,muteButton);
    muteButton->setFixedSize(50,50);

    //Connects the mute button with the relevant slider
    connect(muteButton,SIGNAL(released()),sliders[indexNo],SLOT(muteChannel()));

    //Connects the slider with this object to emit when a channel is muted, passing the index of the channel
    connect(sliders[indexNo],SIGNAL(emitUnmutedChannel(int)),this,SLOT(muteChannelController(int)));

    //Reverse of above
    connect(sliders[indexNo],SIGNAL(emitMutedChannel(int, int)),this,SLOT(unMuteChannelController(int, int)));

    //Add buttons to layout
    muteChannelLayout->setSpacing(0);
    muteChannelLayout->addWidget(muteButton,0,Qt::AlignTop);
    muteChannelLayout->addWidget(channelBox,0,Qt::AlignBottom);

    sliderBoxLayout->addLayout(muteChannelLayout, Qt::AlignRight);
    sliderBoxLayout->addStretch(5000);
}

void Mixer::muteChannelController(int channel)
{
    interface->ChangeVolume(channel, -127);

    mutes[channel]->setStyleSheet("QPushButton { image: url(:/new/prefix1/mute2image.jpg); border: 5px solid black; border-radius:5px;}");
    update();
}

void Mixer::unMuteChannelController(int channel, int volume)
{
    interface->ChangeVolume(channel, volume);

    mutes[channel]->setStyleSheet("QPushButton { image: url(:/new/prefix1/muteimage.jpg); border: 5px solid black; border-radius:5px;}");
    update();

}

void Mixer::constructOutputs() {


    outputFrame = new QDialog();
    numOuts = (usb_mixer_get_num_outputs(0));
    outputLayout = new QVBoxLayout(outputFrame);
    for(int i = 0;i < numOuts; i++)
    {
        OutputBox *outputBox = new OutputBox(i);
        outputBox->setFixedSize(700,150);
        outputBox->setStyleSheet("QCheckBox::indicator { width: 100px; height: 80px; }");
        connect(outputBox,SIGNAL(clicked()), outputBox,SLOT(setOut()));
        connect(outputBox,SIGNAL(emitIndex(int)),this,SLOT(deMapChannel(int)));
        controllerp->mapChannelToMix(i,35+i);
        QString text = "Outputs: ";
        QString text2 = QString::number(i+1);
        outputBox->setText(text.append(text2));
        outputBox->setVisible(false);
        outputs.insert(i,outputBox);
        outputLayout->addWidget(outputBox);
    }


    outputFrame->setStyleSheet("QDialog { border:10px solid black; background-color:white; } ");
    outputFrame->setFixedSize(1000,900);


    hideOutputs = new QPushButton("Done");
    hideOutputs->setFixedSize(200,50);

    connect(hideOutputs,SIGNAL(pressed()),this,SLOT(dePopulateOutputs()));
    connect(hideOutputs,SIGNAL(pressed()),this,SLOT(setOutputs()));


}

void Mixer::deMapChannel(int index)
{
    if(outputs[index]->isChecked() == false)
         controllerp->mapChannelToMix(index, 40);
}

void Mixer::populateOutputs()
{
    outputLayout->addStretch(5000);
    outputLayout->addSpacing(500);
    outputLayout->addWidget(hideOutputs);

    for(int i = 0;i < numOuts;i++)
    {
        if(((controllerp->getMap(i)) < 20) || (controllerp->getMap(i) > 28) || (outputs[i]->isChecked() == true))
        {
            outputs[i]->setVisible(true);
        }
    }
}

void Mixer::dePopulateOutputs()
{
    for(int i = 0;i < numOuts;i++)
        outputs[i]->setVisible(false);
}

void Mixer::setOutputs()
{
    for(int i = 0;i < numOuts;i++)
    {
        if(((controllerp->getMap(i) < 20) || (controllerp->getMap(i) > 28)) && (outputs[i]->isChecked() == true))
        {
            interface->setOutputChannel(i);
        }
    }
    for(int i = 0;i < outputs.size();i++)
    {
        outputs[i]->setVisible(false);
    }
    outputFrame->hide();
}

void Mixer::constructMixer(int output)
{


    //Top Layout
    QHBoxLayout *superLayout = new QHBoxLayout();

    //Frame Object
    QFrame *sliderFingerArea = new QFrame();
    sliderFingerArea->setFixedSize(1500,800);
    sliderFingerArea->setStyleSheet("QFrame { background-color: black; border: 5px solid black; border-radius: 10px; } ");
    //Object to be Scrolled
    QVBoxLayout *mainLayout = new QVBoxLayout();

    //Frame containing master slider
    MasterSlider *mainFrame = new MasterSlider(usb_mixer_get_num_outputs(0), controllerp);


    //FingerSlideObject
    QFingerScrollArea *mix1slide = new QFingerScrollArea(sliderFingerArea);
    mix1slide->setFixedSize(1490,1000);

    //IGNORE
    this->setLayout(superLayout);
    superLayout->addWidget(mainFrame);
    mainFrame->setFixedSize(250,800);


    QFrame *secondFrame = new QFrame();
    secondFrame->setFixedSize(3300,800);
    secondFrame->setLayout(mainLayout);
    superLayout->addWidget(sliderFingerArea);
    superLayout->addStretch(5000);
    superLayout->addSpacing(5000);
    mix1slide->setWidget(secondFrame);


    constructOutputs();
    mainLayout->setSpacing(0);
    QPushButton *outputButton = new QPushButton();
    outputButton->setFixedSize(300,110);
    outputButton->setText("Outputs");
    mainLayout->addWidget(outputButton);

    connect(outputButton,SIGNAL(clicked()),this,SLOT(populateOutputs()));
    connect(outputButton,SIGNAL(clicked()),outputFrame,SLOT(show()));


    volumeLevel = new VolumeLevel(interface, numIns);

    mainLayout->addWidget(volumeLevel,0,Qt::AlignTop);
    volumeLevel->setFixedSize(3200,450);
    volumeLevel->setStyleSheet("background: solid black; border: 10px solid black;");

    //Adjust the main layout's spacing and alignment
    mainLayout->setAlignment(Qt::AlignTop);


    //Create the layout for the sliders and add it to the main layout
    sliderLayout = new QHBoxLayout;


    QGroupBox *sliderFrame = new QGroupBox();

   // mainLayout->addStretch(100);
    mainLayout->addWidget(sliderFrame,Qt::AlignBottom);


    QVBoxLayout *slidervolLayout = new QVBoxLayout();
    sliderFrame->setLayout(slidervolLayout);

    slidervolLayout->addLayout(sliderLayout);
    slidervolLayout->setSpacing(0);
    //slidervolLayout->addStretch(50000);

    sliderFrame->setFixedSize(3200,350);
    sliderFrame->setStyleSheet("QGroupBox {background-color: white; border: 5px solid black; border-radius: 10px; }");


    mainLayout->addStretch(50000);




    //Loop that creates 18 input channels
    for(int i = 0;i < numIns;i++)
    {
        indexNo = i;

        //Each slider starts off as assigned to its relevant channel e.g. slider 1 is assigned to channel 1
        selectedChannel = i;

        //Links up each slider with its relevant channel
        addChannel();

        //Creates the sliders and positions them in the layout
        newSlider();

    }


    //Connects a slider's current value to the interface's change volume function
    connect(this,SIGNAL(emitVolume(int,int)),interface,SLOT(ChangeVolume(int,int)));

    //Sets initial output
    emit emitCurrentOutput(output);
    volumeLevel->startTimer();
    mainFrame->startRefresh();
}

void Mixer::changeVolume(int indexVal, int vol)
{
    emit emitVolume(indexVal, vol);
}















