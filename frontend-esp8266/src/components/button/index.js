import React from 'react';
import {Button, Grid, Segment, Form} from 'semantic-ui-react';
import ButtonDataService from '../../services/dataService';



const CustomButton = ({children}) => {

    const handleOnClick =  async () =>  {
        // var data = {
        //     name: children
        // };
        const response = await ButtonDataService.handleButtonClick(children);
        console.log(children);
        console.log(response);
    };

    return (
        <Grid centered >
                <Grid.Column> 
                    <Segment>
                        <Form>
                            <Form.Field>    
                                <Button
                                    basic
                                    onClick={handleOnClick}
                                    color="pink"    
                                    >
                                {children}
                                </Button>
                            </Form.Field>
                        </Form>
                    </Segment>
                </Grid.Column> 

        </Grid>
    )
};

export default CustomButton;