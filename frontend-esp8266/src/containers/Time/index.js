import React from 'react';
import { Link } from 'react-router-dom';
import AppHeader from '../../components/header';

const TimeContainer = () => {
    return (
        <div>
            <AppHeader />
            <h1>Time Setup</h1>
            <Link to= "/">Home</Link>
        </div>
    );
};

export default TimeContainer;
