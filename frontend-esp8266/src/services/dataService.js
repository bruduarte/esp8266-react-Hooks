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
        const response = await axiosInstance.get("/custompage");
        console.log(response);
        return response.data;
        // let test = [
        //          {
        //              "name": "Button1",
        //              "type": "button"
        //          },
        //          {
        //             "name": "Iput1",
        //             "placeholder":"Input1",
        //             "label":"Input Box One",
        //             "onChange": "alert",
        //             "inputType": "text",
        //             "type": "input",
        //             "handleChange": ""
        //         },
        //          {
        //              "name": "Button2",
        //              "type": "button"
        //          },
        //          {
        //             "name": "Iput2",
        //             "placeholder":"Input2",
        //             "label":"Input Box Two",
        //             "onChange": "alert",
        //             "inputType": "text",
        //             "type": "input",
        //             "handleChange": ""
        //         },
        //         {
        //             "name": "Checkbox1",
        //             "label": "Toggle LED",
        //             "type": "checkbox",
        //         }
            
        //      ]
        //  return test;
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
    catch(error){
        console.error(error);
        return error;
    }
}

const handleButtonClick = async (data) => {
    try {
        const response = await axiosInstance.post("/button", data);
        console.log(response);
        return response.statusText;

    } catch (error) {
        console.error(error);
        return error;
    }
}

const handleSubmitIput = async (data) => {
    try {
        const response = await axiosInstance.post("/inputbox", data);
        console.log(response);
        return response.statusText;
    } catch (error) {
        console.error(error);
        return error;
    }
}

const handleCheckbox = async (data) => {
    try {
        const response = await axiosInstance.post("/checkbox", data);
        console.log(response);
        return response.statusText;
    } catch (error) {
        console.error(error);
        return error;
    }
}



export default {getTime, onSubmitTime, onSubmitCredentials, getCustom, handleButtonClick, handleSubmitIput, handleCheckbox};