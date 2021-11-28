import axios from "axios";




let headers = {"Content-type": "application/json"};

const axiosInstance = axios.create({

    headers,
});

export default axiosInstance;