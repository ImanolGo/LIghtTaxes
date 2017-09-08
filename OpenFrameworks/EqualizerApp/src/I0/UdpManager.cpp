/*
 *  UdpManager.cpp
 *  Equalizer App
 *
 *  Created by Imanol Gomez on 22/08/17.
 *
 */



#include "UdpManager.h"
#include "AppManager.h"
#include "ofxMyIP.h"


const int UdpManager::UDP_MESSAGE_LENGHT = 1000;

const char UdpManager::START_COMMAND = 'd';
const char UdpManager::END_COMMAND = 255;

UdpManager::UdpManager(): Manager(), m_id(0), m_broadcast("")
{
    //Intentionally left empty
}

UdpManager::~UdpManager()
{
   ofLogNotice() << "UdpManager::destructor";
}


//--------------------------------------------------------------

void UdpManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    
    this->setupIP();
    this->setupUdpConnection();
    //this->setupText();
    
    ofLogNotice() <<"UdpManager::initialized" ;
}

void UdpManager::setupUdpConnection()
{
    int portReceive = AppManager::getInstance().getSettingsManager().getUdpPortReceive();
    ofLogNotice() <<"UdpManager::setupUdpReceiver -> listening for udp messages on port  " << portReceive;
    
    //m_udpConnection.SetEnableBroadcast(true);
    m_udpConnection.Create(); //create the socket
    m_udpConnection.Bind(portReceive); //and bind to port
    
    
    string ip = AppManager::getInstance().getSettingsManager().getIpAddress();
    int portSend = AppManager::getInstance().getSettingsManager().getUdpPortSend();
   
    m_udpConnection.Connect(m_broadcast.c_str(),portSend);
    m_udpConnection.SetEnableBroadcast(true);
    
    ofLogNotice() <<"UdpManager::setupUdpReceiver -> sending to IP " << ip <<" to port " << portSend;
    
    m_udpConnection.SetNonBlocking(true);
}


void UdpManager::setupIP()
{
    #ifdef TARGET_WIN32
        system("data/commands/ipfirst.cmd");
        ofFile file("my.ip");
        file >> m_ip;
    #else
        ofxMyIP myip;
        myip.setup();
        m_ip = myip.getIpAddress();
    
    #endif
    
    ofLogNotice() <<"UdpManager::setupIP -> IP address: " << m_ip;
    
    m_broadcast = "";
    auto stringSplit = ofSplitString(m_ip, ".");
    
    for(int i=0; i<stringSplit.size(); i++){
       
        if(i<stringSplit.size()-1){
            m_broadcast += stringSplit[i];
            m_broadcast += ".";
        }
        
        char s = (char) ofToInt(stringSplit[i]);
        m_ipVector.push_back(s);
    }
    
    m_broadcast+="255";
    ofLogNotice() <<"UdpManager::setupIP -> Broadcast IP address: " << m_broadcast;
    
//    for(auto str: stringSplit){
//        char s = (char) ofToInt(str);
//        m_ipVector.push_back(s);
//        m_broadcast = str;
//        //ofLogNotice() <<"UdpManager::setupIP -> IP address: " << s;
//        
//    }
    
}
void UdpManager::setupText()
{
//    auto windowSettings = AppManager::getInstance().getSettingsManager().getWindowsSettings(0);
//    
//    int width =  (windowSettings.width - 4*LayoutManager::MARGIN - GuiManager::GUI_WIDTH)*0.5 - LayoutManager::MARGIN;
//    int fontSize = 12;
//    int height = fontSize*3;
//    
//    ofVec3f position;
//    
//    ofPtr<RectangleVisual> rect = AppManager::getInstance().getOscManager().getBoundingBox();
//    float offset_y = rect->getPosition().y + rect->getHeight();
//    
//    //position.x = GuiManager::GUI_WIDTH + 2*LayoutManager::MARGIN;
//    position.y = LayoutManager::MARGIN + offset_y;
//    position.x = 2.5*LayoutManager::MARGIN + GuiManager::GUI_WIDTH;
//    
//    int porReceive = AppManager::getInstance().getSettingsManager().getUdpPortReceive();
//    string text = ">> UDP receiving -> Port: " + ofToString(porReceive);
//    
//    m_udpReceiveTextFont =  ofPtr<TextVisual> (new TextVisual(position, width, height));
//    m_udpReceiveTextFont->setText(text, "fonts/open-sans/OpenSans-Semibold.ttf", fontSize);
//    m_udpReceiveTextFont->setColor(ofColor::white);
//    m_udpReceiveTextFont->setLineHeight(2.5);
//    
//    position.y += LayoutManager::MARGIN + fontSize;
//    m_udpReceiveMessageFont =  ofPtr<TextVisual> (new TextVisual(position, width, height));
//    m_udpReceiveMessageFont->setText(text, "fonts/open-sans/OpenSans-Semibold.ttf", fontSize);
//    m_udpReceiveMessageFont->setColor(ofColor::white);
//    m_udpReceiveMessageFont->setLineHeight(2.5);
//    
//    AppManager::getInstance().getViewManager().addOverlay(m_udpReceiveTextFont);
//    AppManager::getInstance().getViewManager().addOverlay(m_udpReceiveMessageFont);
//    
//    position.y = LayoutManager::PADDING + offset_y;;
//    position.x -= 0.5*LayoutManager::MARGIN;
//    width += LayoutManager::MARGIN;
//    height = 4*LayoutManager::MARGIN;
//    
//    ofPtr<RectangleVisual> rectangleVisual = ofPtr<RectangleVisual>(new RectangleVisual(position, width, height));
//    ofColor color(60,60,60);
//    //ofColor color = AppManager::getInstance().getSettingsManager().getColor("GUI2");
//    rectangleVisual->setColor(color);
//    
//    AppManager::getInstance().getViewManager().addOverlay(rectangleVisual,2);
}


void UdpManager::update()
{
    char udpMessage[UDP_MESSAGE_LENGHT];
    string message;
    string tempMessage;
    
    //ofLogNotice() <<">>UdpManager::update -> message: " << message;
    //this->updateReceiveText(message);

}


void UdpManager::updateReceiveText(const string& message)
{
    int porReceive = AppManager::getInstance().getSettingsManager().getUdpPortReceive();
    string text = ">> UDP receiving -> Port: " + ofToString(porReceive) ;
    //text += "   " + message;
    m_udpReceiveTextFont->setText(text);
    
    m_udpReceiveMessageFont->setText("   " + message);

}


void UdpManager::sendData(const UdpData& data)
{
    string message="";
    
    message+= START_COMMAND;
    
    char id_ = (char) ofClamp(data.m_id, 0, 254);
    message+= id_;
    
    char data_command = 1;
    message+= data_command;
    
    char strip_nr = (char) ofClamp(data.m_stripNr, 0, 254);
    message+= strip_nr;
    
    char bitmap_nr = (char) ofClamp(data.m_bitmapNr, 0, 254);
    message+= strip_nr;

    char height = (char) ofClamp(data.m_value, 0, 254);
    message+= height;
    
    message+= END_COMMAND;
  
    m_udpConnection.Send(message.c_str(),message.length());
    
    //ofLogNotice() <<"UdpManager::sendData << " << message;
}

void UdpManager::sendLoadBitmap(const UdpData& data)
{
    string message="";
    
    message+= START_COMMAND;
    
    char id_ = (char) ofClamp(data.m_id, 0, 254);
    message+= id_;
    
    char data_command = 0;
    message+= data_command;
    
    char strip_nr = (char) ofClamp(data.m_stripNr, 0, 254);
    message+= strip_nr;
    
    char bitmap_nr = (char) ofClamp(data.m_bitmapNr, 0, 254);
    message+= strip_nr;

    char clear = (char) ofClamp(data.m_value, 0, 1);
    message+= clear;
    
    message+= END_COMMAND;
    
    m_udpConnection.Send(message.c_str(),message.length());
    
    //ofLogNotice() <<"UdpManager::sendData << " << message;
}


void UdpManager::sendSpeed(const UdpData& data)
{
    string message="";
    
    message+= START_COMMAND;
    
    char id_ = (char) ofClamp(data.m_id, 0, 254);
    message+= id_;
    
    char data_command = 2;
    message+= data_command;
    
    char strip_nr = (char) ofClamp(data.m_stripNr, 0, 254);
    message+= strip_nr;
    
    char bitmap_nr = (char) ofClamp(data.m_bitmapNr, 0, 254);
    message+= strip_nr;
    
    char speed = (char) ofClamp(data.m_value, 0, 254);
    message+= speed;
    
    message+= END_COMMAND;
    
    m_udpConnection.Send(message.c_str(),message.length());
    
    //ofLogNotice() <<"UdpManager::sendData << " << message;
}

void UdpManager::sendAutodiscovery()
{
    if(m_ipVector.size()<4){
        return;
    }
    
    string message = "";
    
    message+= START_COMMAND;
    
    message+= 'h';
    
    for(int i=0; i<4; i++)
    {
        message+=m_ipVector[i];
    }
    
    message+= END_COMMAND;
    
    m_udpConnection.Send(message.c_str(),message.length());
    
    //ofLogNotice() <<"UdpManager::sendAutodiscovery";
}






