import React, { useEffect, useState } from "react";
import AppHeader from '../../components/header';
import CustomButton from '../../components/button';
import CustomDataService from '../../services/dataService';
import { Grid } from "semantic-ui-react";


const CustomContainer = () => {
    const [data, setData] = useState([]);

    const getCustomData = () => {
        CustomDataService.getCustom().then(res => setData(res));
    }
    
    useEffect(()=>{
        getCustomData()
      },[])

      debugger;
    return(
        <div>
            <AppHeader />
            <Grid centered>
                {
                    data && data.length > 0 && data.map((item) => {
                        return (
                            <Grid.Row>
                                if(item == "Button"){
                                    <CustomButton onClick={() => alert (item.name)} > item.name </CustomButton>
                                }
                                else if(item == "Input"){
        
                                }
                                else if(item == "Checkbox"){
        
                                }
                            </Grid.Row>

                        )
                    })
                }
                <CustomButton 
                    type= 'submit'
                    onClick = {() => alert("Button 1 clicked")}
                >
                    Teste
                </CustomButton>
                <CustomButton 
                    type= 'submit'
                    onClick = {() => alert("Button 2 clicked")}
                >
                    Teste2
                </CustomButton>
            </Grid>

        </div>
    )
};

export default CustomContainer;