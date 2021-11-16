import React from 'react';
import {Button, Grid, Segment, Form} from 'semantic-ui-react';



const CustomButton = ({children, onClick}) => {
    return (
        <Grid centered >
                <Grid.Column> 
                    <Segment>
                        <Form>
                            <Form.Field>    
                                <Button
                                    basic
                                    onClick={onClick}
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