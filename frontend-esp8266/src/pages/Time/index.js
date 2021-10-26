import React, { useState, useReducer } from 'react';
import { Grid, Segment, Header, Form, Button, Message, Checkbox } from 'semantic-ui-react';
import AppHeader from '../../components/header';
import TimeDataService from "../../services/dataService";
import timezones from '../../assets/timezones';
import DatePicker from 'react-datepicker';
import "react-datepicker/dist/react-datepicker.css";



const TimeContainer = () => {

    const [input, setInput] = useState(0);
    const [error, setError] = useState("");
    const [checked, toggle] = useReducer((checked) => !checked, false);
    const [startDateTime, setStartDateTime] = useState(null);
    const [endDateTime, setEndDateTime] = useState(null);
    // const [submit, setSubmi] = useState(false);

    const handleDropdownChange = ( event, data ) => {
        setInput({[data.name]:data.value});
        console.log({[data.name]:data.value});
    };

    const handleStartDateTimeChange = (date) => {
        
        setStartDateTime(date)
        console.log(date);
    };

    const handleEndDateTimeChange = (date) => {
        
        setEndDateTime(date)
        console.log(date);
    };

    const saveTimeConfig = () => {
        let startTime = null;
        let endTime = null;

        if (checked && startDateTime != null){
            startTime = startDateTime.getTime();
        }
        if (checked && endDateTime != null){
            endTime = endDateTime.getTime();
        }

        var data = {
            timeOffset: input.timeOffset,
            daylightSavingTime: checked,
            startDateTime: startTime,
            endDateTime: endTime,
        };
    

        TimeDataService.onSubmitTime(data)
            .then(response => {
                setInput({
                    timeOffset: response.data.timeOffset,
                    daylightSavingTime: response.data.daylightSavingTime,
                    startDateTime: response.data.startDateTime,
                    endDateTime: response.data.endDateTime,
                });

                console.log(response.data);
            })
            .catch((e) => {
                console.log(e.data);
                setError(e.data);

            });
        
    };

    const submitFormValid = (input !== 0) && ((checked && startDateTime && endDateTime) || (!checked && !startDateTime && !endDateTime));

    // const handleSubmit = (event) => {
    //     event.target = [{}];

        
    // };


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
                            <Form.Field>
                                <DatePicker 
                                    placeholderText="Select DST Start Date" 
                                    disabled={!checked} 
                                    selected={startDateTime} 
                                    onChange={handleStartDateTimeChange} 
                                    showTimeSelect 
                                    timeFormat="HH:mm"
                                    timeIntervals={15} 
                                    isClearable={true} 
                                />
                                <DatePicker 
                                    placeholderText="Select DST End Date" 
                                    disabled={!checked} 
                                    selected={endDateTime} 
                                    onChange={handleEndDateTimeChange} 
                                    showTimeSelect 
                                    timeFormat="HH:mm"
                                    timeIntervals={15}
                                    isClearable={true} 
                                />
                            </Form.Field>
                            <Button
                                onClick={saveTimeConfig} 
                                fluid
                                color="teal" 
                                type='submit'
                                disabled={!submitFormValid}
                            >Submit
                            </Button>
                        </Form>
                    </Segment>
                </Grid.Column>
            </Grid>
        </div>
        
    );
};

export default TimeContainer;
