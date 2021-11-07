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



export default {getTime, onSubmitTime, onSubmitCredentials};