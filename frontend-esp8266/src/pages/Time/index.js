import React, { useState } from 'react';
import { Grid, Segment, Header, Form, Button } from 'semantic-ui-react';
import AppHeader from '../../components/header';
import TimeDataService from "../../services/dataService";

const TimeContainer = () => {

    const initialTimezone = {
        area: "",
        city: "",

    };

    const [timezone, setTimezone] = useState(initialTimezone);


    const handleInputTimezone = event => {
        const { name, value } = event.target;
        setTimezone({...timezone, [name]: value});
        console.log(name, value);
    };

    const saveTimeConfig = () => {
        var data = {
            area: timezone.area,
            city: timezone.city
        };
    

        TimeDataService.onSubmitTime(data)
            .then(response => {
                setTimezone({
                    area: response.data.area,
                    city: response.data.city,
                });

                console.log(response.data);
            })
            .catch(e => {
                console.log(e);
            });
        
    };


    return (
        <div>
            <AppHeader />

            <Grid centered>
                <Grid.Column style={{maxWidth: 550, marinTop: 20}}>
                    <Header textAlign='center'>Time Setup</Header>
                    <Segment>
                        <Form>
                            <Form.Field>
                                <Form.Input
                                    value={timezone.area}
                                    onChange={handleInputTimezone}
                                    name="area"
                                    placeholder="please enter your region"
                                    label="Region"
                                    
                                />
                            </Form.Field>
                            <Form.Field>
                                <Form.Input 
                                    value={timezone.city}
                                    onChange={handleInputTimezone}
                                    name="city" 
                                    type="city" 
                                    placeholder="please enter your city" 
                                    label="City"

                            />
                            </Form.Field>
                            <Button 
                                onClick={saveTimeConfig} 
                                fluid
                                color="teal" 
                                type='submit'>Submit
                            </Button>
                        </Form>
                    </Segment>
                </Grid.Column>
            </Grid>
        </div>
        
    );
};

export default TimeContainer;
