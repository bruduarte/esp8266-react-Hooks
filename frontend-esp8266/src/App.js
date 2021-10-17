import logo from "./assets/images/logo.svg";
import './App.css';
import {BrowserRouter as Router, Switch, Route } from 'react-router-dom';
import routes from './routes';

function App() {
  return (
    <div className="App">
      <Router>
        <Switch>
            {routes.map((route, index) => (
              <Route
                key={index}
                path={route.path}
                exact
                render={(props) => <route.component {...props}/>}
              ></Route>
            ))}
        </Switch>
      </Router>
    </div>
  );
}

export default App;
