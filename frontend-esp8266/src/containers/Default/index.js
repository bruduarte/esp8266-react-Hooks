import React from 'react';
import { Link } from 'react-router-dom';
import AppHeader from '../../components/header';

const DefaultContainer = () => {
    return (
        <div>
            <AppHeader />
            <h1 style={{fontSize: 60}}>Welcome :)</h1>
            <h2>You can start the setup choosing from the Menu bar!</h2>
        </div>
    );
};

export default DefaultContainer;
