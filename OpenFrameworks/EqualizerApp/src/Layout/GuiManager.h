/*
 *  GuiManager.h
 *  Equalizer App
 *
 *  Created by Imanol Gomez on 22/08/17.
 *
 */


#pragma once

#include "Manager.h"
#include "ofxGui.h"
//========================== class GuiManager ==============================
//============================================================================
/** \class GuiManager GuiManager.h
 *	\brief Class managing the application´s grapical user interface (GUI)
 *	\details It creates the gui and the callback functions that will be affected
 *    by the gui
 */

class GuiManager: public Manager
{
    static const string GUI_SETTINGS_FILE_NAME;
    static const string GUI_SETTINGS_NAME;
    static const int GUI_WIDTH;
    
public:

    //! Constructor
    GuiManager();

    //! Destructor
    ~GuiManager();

    //! Set-up the gui
    void setup();
    
    //! Draw the gui
    void draw();
    
    void saveGuiValues();
    
    void loadGuiValues();
    
    void toggleGui();
    
    void showGui(bool show){m_showGui=show;}
    
    void onSetVideoMode(bool& value);
    
    void onSetNoiseMode(bool& value);
    
    void onSetAudioMode(bool& value);
    
    int getWidth() {return GUI_WIDTH;}
    
    int getHeight() {return m_gui.getHeight();}
    
    ofPoint  getPosition() {return m_gui.getPosition();}
    
    ofxColorSlider& getColor(){return m_color;}
    
private:
    
    void setupGuiParameters();
    
    void setupLayoutGui();
    
    void setupCommunicationsGui();
    
    void setupNoiseGui();
    
    void drawRectangle();

private:
    
    // Fluid GUI
    ofxPanel			m_gui;
    
    ofParameter<float>	m_guiFPS;
    

    ofParameterGroup    m_parametersLayout;
    ofParameterGroup    m_parametersNoise;
    ofParameterGroup    m_parametersCommunications;
    
    ofParameter<int>      m_id;
    
    ofParameter<int>      m_noiseResolution;
    ofParameter<float>    m_noiseFrequency;
    ofParameter<float>    m_noiseSpeed;
    
    ofParameter<int>      m_comBitmapNum;
    ofParameter<int>      m_comStripNum;
    ofParameter<bool>     m_comClearLights;
    ofParameter<bool>     m_comSendHeights;
    ofParameter<int>      m_comSpeed;
    
    ofxColorSlider          m_color;
    
   
    bool        m_showGui;  //It defines the whether the gui should be shown or not
};

//==========================================================================


