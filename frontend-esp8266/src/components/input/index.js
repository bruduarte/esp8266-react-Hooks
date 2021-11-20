import React, {useState} from 'react';
import { Grid, Form, Segment, Message, Button } from 'semantic-ui-react';
import InputDataService from '../../services/dataService';


const CustomInput = ({placeholder, name, type, label}) => {

    const [input, setInput] = useState('');
    const [error, setError] = useState(false);

    const handleChange = (event) => {
        const {value}  = event.target;
        setInput(value);
    };
    console.log(input);

    const saveVariable = async () => {
        var data = name + " " + input;
  
        const response = await InputDataService.handleSubmitIput(data);
        console.log(response);
        if(response !== "OK"){
            setError(true);
        }else{
            setError(false);
        }
    };
    
    return(
        <Grid centered>
            <Grid.Column style={{maxWidth: 500, marginTop: 40}}>
                <Segment>
                    <Form onSubmit={saveVariable}> 
                    {error && <Message header="Something went wrong :( " negative/>}
                        <Form.Field>
                            <Form.Input 
                                value={input} 
                                onChange={handleChange}
                                placeholder={placeholder} 
                                name={name}
                                label= {label}
                                type={type}
                            />
                        </Form.Field>
                        <Button  
                                fluid
                                color="purple" 
                                type='submit'
                                disabled={!input}
                        >Submit
                        </Button>
                    </Form>
                </Segment>
            </Grid.Column>
        </Grid>
    )
};

export default CustomInput;
