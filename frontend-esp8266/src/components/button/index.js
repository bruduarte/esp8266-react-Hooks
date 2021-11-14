import React from 'react';
import {Button, Grid} from 'semantic-ui-react';



const CustomButton = ({children, onClick}) => {
    return (
        <Grid centered columns='equal' >
            <Grid.Row>
                <Grid.Column> 
                    <Button
                        basic
                        onClick={onClick}
                        color="pink"    
                    >
                    {children}
                    </Button>
                </Grid.Column> 
            </Grid.Row>
        </Grid>
    )
};

export default CustomButton;