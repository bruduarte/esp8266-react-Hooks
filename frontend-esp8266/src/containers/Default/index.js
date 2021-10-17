import React from 'react';
import { Link } from 'react-router-dom';

const DefaultContainer = () => {
    return (
        <div>
            <h1>ESP8266</h1>
            <Link to= "/wificonfiguration">Setup WiFi</Link>
            <Link to="/time">Setup Time</Link>
        </div>
    );
};

export default DefaultContainer;
