import React from 'react';
import {Button, Grid, Segment} from 'semantic-ui-react';



const CustomButton = ({children, onClick}) => {
    return (
        <Grid centered columns='equal' >

                <Grid.Column> 
                    <Segment>
                        <Button
                            basic
                            onClick={onClick}
                            color="pink"    
                        >
                        {children}
                        </Button>
                    </Segment>
                </Grid.Column> 

        </Grid>
    )
};

export default CustomButton;