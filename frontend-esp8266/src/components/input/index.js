import React, {useState} from 'react';
import { Grid, Form, Segment } from 'semantic-ui-react';


const CustomInput = ({placeholder, name, type}) => {

    const [input, setInput] = useState('');

    const handleChange = (event) => {
        const {value}  = event.target;
        setInput(value);
        console.log(input);
    };

    return(
        <Grid centered>
            <Grid.Column style={{maxWidth: 700, marginTop: 20}}>
                <Segment>
                    <Form > 
                        <Form.Field>
                            <Form.Input 
                                value={input} 
                                onChange={handleChange}
                                placeholder={placeholder} 
                                name={name}
                                label= {name}
                                type={type}
                            />
                        </Form.Field>
                    </Form>
                </Segment>
            </Grid.Column>
        </Grid>
    )
};

export default CustomInput;
