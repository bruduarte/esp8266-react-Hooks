import React, { useEffect } from 'react';
import { Link } from 'react-router-dom';
import AppHeader from '../../components/header';
import { wifi } from '../../context/actions/configwifi';

const WifiContainer = () => {
    //using useEffect here just to test the action described by the function "wifi" which is an API post request. 
    useEffect(()=>{
        wifi();
    },[]);

    return (
        <div>
            <AppHeader />
            <h1 style={{fontSize: 30}}>WiFi Configuration</h1>
            
        </div>
    );
};

export default WifiContainer;
