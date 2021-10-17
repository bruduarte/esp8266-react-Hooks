/*routes configurations*/
import WifiComponent from "../containers/Wifi";
import TimeComponent from "../containers/Time";
import DefaultComponent from "../containers/Default";

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