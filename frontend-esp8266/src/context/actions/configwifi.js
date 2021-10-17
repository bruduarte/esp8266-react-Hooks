/*this file handles the API calls*/

import axiosInstance from "../../helpers/axios";

export const wifi = () => {
    axiosInstance.post("/wificonfiguration")
        .then((response => console.log("response: ", response)))
        .catch((e) => console.log("error: ", e));
};