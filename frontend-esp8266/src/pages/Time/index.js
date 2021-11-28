import React, { useState, useReducer } from 'react';
import { Grid, Segment, Header, Form, Button, Message, Checkbox } from 'semantic-ui-react';
import AppHeader from '../../components/header';
import TimeDataService from "../../services/dataService";
import timezones from '../../assets/timezones';
import DatePicker from 'react-datepicker';
import "react-datepicker/dist/react-datepicker.css";



const TimeContainer = () => {

    const initialState = {
        timeOffset: '',
        checked: false,
        startDateTime: null,
        endDateTime: null,
    }

    const [input, setInput] = useState(initialState.timeOffset);
    const [error, setError] = useState(false);
    const [checked, toggle] = useReducer((checked) => !checked, initialState.checked); //const [state, dispatch] = useReducer(reducer, initialState);
    const [startDateTime, setStartDateTime] = useState(initialState.startDateTime);
    const [endDateTime, setEndDateTime] = useState(initialState.endDateTime);
    

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

    const saveTimeConfig = async () => {
        let startTime = "";
        let endTime = "";

        if (checked && startDateTime != null){
            startTime = startDateTime.getTime();
        }
        if (checked && endDateTime != null){
            endTime = endDateTime.getTime();
        }

        var data = {
            ntpOffset: input.timeOffset,
            ntpDSTenabled: checked,
            ntpDSTstartDate: startTime.toString(),
            ntpDSTendDate: endTime.toString(),
        };
    

        const response = await TimeDataService.onSubmitTime(data);
        console.log(response);
        if(response !== "OK"){
            setError(true);
        }
        
    };

    const submitFormValid = (input !== null) && ((checked && startDateTime && endDateTime) || (!checked && !startDateTime && !endDateTime));



    return (
        <div>
            <AppHeader />

            <Grid centered>
                <Grid.Column style={{maxWidth: 550, marginTop: 20}}>
                    <Header textAlign='center'>Timezone Setup</Header>
                    <Segment>
                        <Form onSubmit={saveTimeConfig}> 
                            {error && <Message header="Something went wrong! :(" negative/>}
                            <Form.Field>
                                <label>Timezone</label>
                                <Form.Select 
                                    fluid
                                    value={input.timezone} 
                                    placeholder="Please, select your timezone"  
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
                                    defaultChecked={false}
                                    
                                />
                            </Form.Field>
                            <Form.Field>
                                <label>DST Start Date</label>
                                <DatePicker 
                                    name="startDateTime"
                                    placeholderText="Select DST Start Date" 
                                    disabled={!checked} 
                                    selected={startDateTime} 
                                    onChange={handleStartDateTimeChange} 
                                    showTimeSelect 
                                    timeFormat="HH:mm"
                                    timeIntervals={15} 
                                    isClearable={true} 
                                    
                                />
                                <label>DST End Date</label>
                                <DatePicker 
                                    name="endDateTime"
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
                                 
                                fluid
                                color="orange" 
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

export default TimeContainer;
