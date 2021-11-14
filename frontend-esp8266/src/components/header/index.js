import React from 'react';
import { Link } from 'react-router-dom';
import { Button, Icon, Menu } from 'semantic-ui-react';
import { GiCircuitry} from "react-icons/gi";


const AppHeader = () => {
    return (
        <Menu secondary pointing>
            <GiCircuitry size={50} style={{ fill: 'orange' }} />
            <Menu.Item as={Link} to="/" style={{fontSize: 24}}>ESP8266</Menu.Item>
            <Menu.Item position="right">
                <Button as={Link} to="/wificonfiguration" basic icon color="teal">
                    <Icon name="wifi"></Icon>
                    Setup WiFi
                </Button>
            </Menu.Item>
            <Menu.Item>
                {" "}
                <Button as={Link} to="/time" basic icon color="orange">
                    <Icon name="clock outline"></Icon>
                    Time Setup
                </Button>
            </Menu.Item>
            <Menu.Item>
                {" "}
                <Button as={Link} to="/custompage" basic icon color="purple">
                    <Icon name="paper plane outline"></Icon>
                    Custom Page
                </Button>
            </Menu.Item>
        </Menu>
    );
};

export default AppHeader;
