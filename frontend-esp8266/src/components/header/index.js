import React from 'react';
import { Link } from 'react-router-dom';
import { Button, Icon, Image, Menu } from 'semantic-ui-react';
import circuit from "../../assets/images/circuit.svg";

const AppHeader = () => {
    return (
        <Menu secondary pointing>
            <Image src={circuit} width={50} as={Link} to="/"></Image>
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
        </Menu>
    );
};

export default AppHeader;
