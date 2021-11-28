import React, { useEffect, useState } from "react";
import AppHeader from '../../components/header';
import CustomButton from '../../components/button';
import CustomDataService from '../../services/dataService';
import CustomInput from "../../components/input";
import CustomCheckbox from "../../components/checkbox";


const CustomContainer = () => {
    const [data, setData] = useState([]);

    
    const getCustomData = () => {
        CustomDataService.getCustom().then(res => setData(res));

    }
    
    useEffect(()=>{
        getCustomData();
      }, []);
    
    console.log(data);


    return(
        <div>
            <AppHeader />
            
                {
                    data && data.length > 0 && data.map((pageItem) => {
                        return (
                            pageItem.type === 'input' ? <CustomInput
                                                            placeholder={pageItem.placeholder}
                                                            name={pageItem.name} 
                                                            label={pageItem.label} 
                                                            type={pageItem.inputType}
                                                        /> :
                            pageItem.type === 'button' ? <CustomButton >{pageItem.name}</CustomButton> : 
                            pageItem.type === 'checkbox' ? <CustomCheckbox  name={pageItem.name} 
                                                                            label={pageItem.label}
                                                                            type={pageItem.type}
                                                                            defaultChecked={pageItem.defaultChecked}
                                                            /> : <>No valid type received!</>

                        );
                    }, this)
                }

        </div>
    )
};

export default CustomContainer;