/*this file handles the API calls*/

import axiosInstance from "../helpers/axios";

const getTime = async () => {
    try{
        const response = await axiosInstance.get("/Timezone");
        console.log(response);
    }
    catch(e){
        console.error(e);
    }
    // return axiosInstance.get("/Timezone");
};

const getCustom = async () => {
    try{
        // const response = await axiosInstance.get("/pageDescriptor");
        // console.log(response);
        let test = [
                 {
                     "name": "Button1",
                     "onClick": "submit",
                     "type": "button"
                 },
                 {
                    "name": "Iput1",
                    "placeholder":"Input1",
                    "onChange": "alert",
                    "inputType": "text",
                    "type": "input",
                    "handleChange": ""
                },
                 {
                     "name": "Button2",
                     "onClick": "/toggle",
                     "type": "button"
                 },
                 {
                    "name": "Iput2",
                    "placeholder":"Input2",
                    "onChange": "alert",
                    "inputType": "text",
                    "type": "input",
                    "handleChange": ""
                },
                {
                    "name": "Checkbox1",
                    "onClick": "alert",
                    "type": "checkbox"
                }
            
             ]
         
        return test;
    }
    catch(e){
        console.error(e);
    }
}

const onSubmitTime = async (data) => {
    try{
        //"/config/update"
        const response = await axiosInstance.post("/Timezone", data);
        console.log(response.statusText);
        return response.statusText;
    }
    catch(e){
        console.error(e);
        return e;
    }
}

const onSubmitCredentials = async (data) => {
    try{
        const response = await axiosInstance.post("/Wifi", data);
        console.log(response);
        return response.statusText;
    }
    catch(e){
        console.error(e);
        return e;
    }
}



export default {getTime, onSubmitTime, onSubmitCredentials, getCustom};