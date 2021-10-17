/*Application State Provider */
import { createContext, useReducer } from "react";

export const GlobalContext = createContext({});
//provides states to other components 
export const GlobalProvider = ({ children }) => {

    //Message:  define states of the application *** talvez n precise desse arquivo*** no exemplo ele tem authentication e contacts states
    //this part of the code tells how our state changes
    //our states changes through Reducers!
}