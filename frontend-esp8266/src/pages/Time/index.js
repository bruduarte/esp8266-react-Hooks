import React, { useState, useReducer } from 'react';
import { Grid, Segment, Header, Form, Button, Message, Checkbox } from 'semantic-ui-react';
import AppHeader from '../../components/header';
import TimeDataService from "../../services/dataService";
import timezones from '../../assets/timezones';


const TimeContainer = () => {

    const [input, setInput] = useState(0);
    const [error, setError] = useState("");
    const [checked, toggle] = useReducer((checked) => !checked, false);


    const handleDropdownChange = ( event, data ) => {
        setInput({[data.name]:data.value});
        console.log({[data.name]:data.value});
    };

    const saveTimeConfig = () => {
        var data = {
            timeOffset: input.timeOffset,
            daylightSavingTime: checked,
        };
    

        TimeDataService.onSubmitTime(data)
            .then(response => {
                setInput({
                    timeOffset: response.data.timeOffset,
                    daylightSavingTime: response.data.daylightSavingTime,
                });

                console.log(response.data);
            })
            .catch((e) => {
                console.log(e.data);
                setError(e.data);
                
            });
        
    };


    return (
        <div>
            <AppHeader />

            <Grid centered>
                <Grid.Column style={{maxWidth: 550, marinTop: 20}}>
                    <Header textAlign='center'>Timezone Setup</Header>
                    <Segment>
                        <Form>
                            {error && <Message header="Error" content={error} error/>}
                            <Form.Field>
                                <label>Timezone</label>
                                <Form.Dropdown
                                    fluid
                                    value={input.timezone} 
                                    placeholder="Please, select your timezone" 
                                    clearable 
                                    options={timezones} 
                                    selection 
                                    name="timeOffset" 
                                    onChange={handleDropdownChange}
                                />    

                            </Form.Field>
                            <Form.Field>
                                <Checkbox 
                                    label="Daylight Saving Time?"
                                    name="daylightSavingTime"
                                    onClick={toggle}
                                    
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
