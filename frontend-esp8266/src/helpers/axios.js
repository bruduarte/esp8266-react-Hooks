import axios from "axios";

const baseURL = process.env.REACT_APP_BACKEND_URL;

console.log("baseURL: ",baseURL);
let headers = {"Content-type": "application/json"};

const axiosInstance = axios.create({
    baseURL: baseURL,
    headers,
});

export default axiosInstance;