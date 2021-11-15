import React, { useEffect, useState } from "react";
import AppHeader from '../../components/header';
import CustomButton from '../../components/button';
import CustomDataService from '../../services/dataService';
import { Grid } from "semantic-ui-react";


const CustomContainer = () => {
    const [data, setData] = useState([]);

    
    const getCustomData = () => {
        CustomDataService.getCustom().then(res => setData(res));
        // setData(test);

    }
    
    useEffect(()=>{
        getCustomData();
      }, []);
    
    console.log(data);

    // debugger;

    return(
        <div>
            <AppHeader />
            
                {
                    data && data.length > 0 && data.map((pageItem) => {
                        return (
                            pageItem.type === 'button' ? <CustomButton onClick = {pageItem.onClick} >{pageItem.name}</CustomButton> : 
                            pageItem.type === 'input' ? <p>{pageItem.name}</p> : 
                            pageItem.type === 'checkbox' ? <p>{pageItem.name}</p> : <>Nada</>

                        );
                    }, this)
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
    

        </div>
    )
};

export default CustomContainer;