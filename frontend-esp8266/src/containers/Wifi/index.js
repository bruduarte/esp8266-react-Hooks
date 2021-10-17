import React, { useEffect } from 'react';
import { Link } from 'react-router-dom';
import AppHeader from '../../components/header';
import { wifi } from '../../context/actions/configwifi';

const WifiContainer = () => {
    //using useEffect here just to test the action described by the function "wifi". 
    useEffect(()=>{
        wifi();
    },[]);

    return (
        <div>
            <AppHeader />
            <h1>WiFi Configuration</h1>
            <Link to="/">Home</Link>
        </div>
    );
};

export default WifiContainer;
