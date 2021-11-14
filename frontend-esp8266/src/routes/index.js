/*routes configurations*/
import WifiComponent from "../pages/Wifi";
import TimeComponent from "../pages/Time";
import CustomComponent from "../pages/Custom";
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
        path:"/custompage",
        component: CustomComponent,
        title:"Customized Page",
    },
    {
        path:"/",
        component: DefaultComponent,
        title:"ESP8266",
    },
];

export default routes;