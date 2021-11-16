import React, {useReducer} from 'react';
import { Grid, Segment, Form, Checkbox } from 'semantic-ui-react';

const CustomCheckbox = ({label, name, type}) => {

    
    const [checked, toggle] = useReducer((checked) => !checked, false); //const [state, dispatch] = useReducer(reducer, initialState);



    return (


            <Grid centered>
                <Grid.Column style={{maxWidth: 550, marginTop: 20}}>
                    <Segment>
                        <Form > 
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