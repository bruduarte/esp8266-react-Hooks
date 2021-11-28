import React, {useState} from 'react';
import { Grid, Segment, Header, Form, Button, Message } from 'semantic-ui-react';
import AppHeader from '../../components/header';
import WifiDataService from "../../services/dataService";



const WifiContainer = () => {
    const initialState = {
        wifiSSID: '',
        wifiPassword: '',
     
    }

    const [input, setInput] = useState(initialState);
    const [error, setError] = useState(false);

    const handleChange = (event) => {
        const {name, value } = event.target;
        setInput({...input, [name]: value});
    };

    const saveWifiConfig = async () => {
        
        var data = {
            wifiSSID: input.wifiSSID,
            wifiPassword: input.wifiPassword,
        };
    

        const response = await WifiDataService.onSubmitCredentials(data);
        console.log(response);
        if(response !== "OK"){
            setError(true);
        }else{
            setError(false);
        }
    };

    const submitFormValid = (input.wifiPassword !== "" && input.wifiSSID !== "");

    return (
        <div>
            <AppHeader />
            <Grid centered>
                <Grid.Column style={{maxWidth: 550, marginTop: 20}}>
                    <Header textAlign='center'>WiFi Setup</Header>
                    <Segment>
                        <Form onSubmit={saveWifiConfig}> 
                            {error && <Message header="Something went wrong :( " negative/>}
                            <Form.Field>
                                <Form.Input 
                                    value={input.wifiSSID || ''} 
                                    onChange={handleChange}
                                    placeholder="Please, type your SSID"  
                                    name="wifiSSID"
                                    label= "SSID"
                                    type="text"
                                />
                            </Form.Field>
                            <Form.Field>
                                <Form.Input 
                                    value={input.wifiPassword || ''} 
                                    placeholder="Please, type your password"  
                                    name="wifiPassword"
                                    type="password"
                                    label= "Password" 
                                    onChange={handleChange}
                                />
                            </Form.Field>
                            
                            <Button  
                                fluid
                                color="teal" 
                                type='submit'
                                disabled={!submitFormValid || !input}
                            >Submit
                            </Button>
                        </Form>
                    </Segment>
                </Grid.Column>
            </Grid>
            
        </div>
    );
};

export default WifiContainer;
