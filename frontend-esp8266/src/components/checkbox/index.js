import React, {useEffect, useReducer, useState} from 'react';
import { Grid, Segment, Form, Checkbox, Message } from 'semantic-ui-react';
import CheckboxDataService from '../../services/dataService';

const CustomCheckbox = ({label, name, type}) => {


    const [checked, toggle] = useReducer((checked) => !checked, false); //const [state, dispatch] = useReducer(reducer, initialState);
    const [error, setError] = useState('');


    console.log(checked);
    
    const setVariable = async () => {
        var data = name + " " + checked;
        console.log(data);
                
        const response = await CheckboxDataService.handleCheckbox(data);
        console.log(response);
        if(response !== "OK"){
            setError(response);
        }else{
            setError('');
        }    
    }

    useEffect(()=>{
        setVariable();
    }, [checked]);

    return (


            <Grid centered>
                <Grid.Column style={{maxWidth: 500, marginTop: 20}}>
                    <Segment>
                        <Form > 
                        {error && <Message header="Something went wrong :( " size={'small'} negative/>}
                            <Form.Field>
                                <Checkbox 
                                    checked = {checked}
                                    label={label}
                                    name={name}
                                    onClick={toggle}
                                    type={type}
                                    
                                />
                            </Form.Field>
                        </Form>
                    </Segment>
                </Grid.Column>
            </Grid>

        
    );
};

export default CustomCheckbox;