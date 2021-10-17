import React from 'react';
import { Link } from 'react-router-dom';
import AppHeader from '../../components/header';

const DefaultContainer = () => {
    return (
        <div>
            <AppHeader />
            <h1>Welcome</h1>
            <Link to= "/wificonfiguration">Setup WiFi</Link>
            <Link to="/time">Setup Time</Link>
        </div>
    );
};

export default DefaultContainer;
