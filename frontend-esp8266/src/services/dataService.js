/*this file handles the API calls*/

import axiosInstance from "../helpers/axios";

const getTime = () => {
    return axiosInstance.get("/Timezone");
};

const onSubmitTime = (data) => {
    return axiosInstance.post("/Timezone", data);
}



export default {getTime, onSubmitTime};