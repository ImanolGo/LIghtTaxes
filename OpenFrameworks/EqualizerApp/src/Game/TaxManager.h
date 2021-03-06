/*
 *  TaxManager.h
 *  Equalizer App
 *
 *  Created by Imanol Gomez on 19/09/17.
 *
 */



#pragma once

#include "ofMain.h"
#include "Manager.h"
#include "TaxCitizen.h"




//========================== struct TaxBand =======================================
//==============================================================================
/** \struct TaxBand TaxManager.h
 *	\brief struct definding a tax band variables
 */


struct TaxBand {
    int type = 0;
    string name = "";
    float min = 0.0;
    float max = 0.0;
    float rate = 0;
};


//========================== class TaxManager =======================================
//==============================================================================
/** \class TaxManager TaxManager.h
 *	\brief class for managing the taxes
 *	\details It sets the taxes game and calculates the result
 */


class TaxManager: public Manager
{
    
    static const double REFRESH_TIME; ///< holds the refresh update time for every citizen update
    static const int MAX_NUM_YEARS; ///< holds the maximum number of years to make the simulation

public:
    //! Constructor
    TaxManager();

    //! Destructor
    virtual ~TaxManager();

    //! setups the tax manager
    void setup();
    
    //! updates the tax manager
    void update();
    
    void onSetBasicIncome(float& value);
    
    void onSetDirectTaxRate(float& value);
    
    void onSetBasicTaxRate(float& value) {this->setIncomeTaxRate(value, 1);}
    
    void onSetHigherTaxRate(float& value){this->setIncomeTaxRate(value, 2);}
    
    void onSetAdditionalTaxRate(float& value){this->setIncomeTaxRate(value, 3);}


private:
    
    //! Loads the settings file
    bool loadSettingsFile();
    
    //! Loads all the settings
    void loadAllSettings();
    
    void readTaxBands();
    
    void readTaxConditions();
    
    void setupCitizens();
    
    float getTaxRate(float income);
    
    float getTaxRandomIncome(int type);
    
    void updateCitizens();
    
    void updateHeights();
    
    void setIncomeTaxRate(float value, int type);
    
    
private:
    
    typedef map<int, shared_ptr<TaxCitizen> > CitizenMap;
    typedef vector<TaxBand>  TaxBandList;
    
    CitizenMap      m_citizens;
    TaxBandList     m_taxBands;
    CitizenSettings m_settings;
    
    ofXml           m_xml;
    
    double          m_elapsedTime;
    int             m_numYears;
    


};

