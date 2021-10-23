/*routes configurations*/
import WifiComponent from "../pages/Wifi";
import TimeComponent from "../pages/Time";
import DefaultComponent from "../pages/Default";

const routes= [
    {
        path:"/wificonfiguration",
        component: WifiComponent,
        title:"WiFi Configuration",
    },
    {
        path:"/time",
        component: TimeComponent,
        title:"Time Setup",
    },
    {
        path:"/",
        component: DefaultComponent,
        title:"ESP8266",
    },
];

export default routes;