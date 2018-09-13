/*open BsReactstrap;

  let component = ReasonReact.statelessComponent("SomeComponent");

  let make = (~onChange, _children) => {
    ...component,

    render: _self => {
      <Button color="primary" size="lg" onClick=(_e => Js.log("Hi!"))>
          (ReasonReact.string("Hello"))
    </Button>
    }
  };*/

let x = ReasonApollo.Provider.make;

[%bs.raw {|require('./App.css')|}];
type state = {
  clientData: option(array(ClientData.client)),
  inputName: option(string),
  inputSheet: option(string),
  isLoggedin: bool,
};
type action =
  | Submit(array(ClientData.client))
  | InputNameChange(string)
  | InputSheetChange(string)
  | Login
  | ClickAnimation;

let component = ReasonReact.reducerComponent("App");

let dummyClients: array(ClientData.client) = [|
  {
    name: "Yuki",
    sheet: "https://google.com.example",
    applicationId: "A12345",
    time: "9/1/2018",
  },
  {
    name: "YAN",
    sheet: "https://google.com.example",
    applicationId: "A12345555",
    time: "9/3/2018",
  },
|];

let make = _children => {
  ...component,
  initialState: () => {
    clientData: Some(dummyClients),
    inputName: None,
    inputSheet: None,
    isLoggedin: false,
  },
  reducer: (action, state) =>
    switch (action) {
    | Submit(loadedClient) =>
      Js.log("clicked");
      Js.log(state.inputName);
      Js.log(state.inputSheet);
      ReasonReact.Update({...state, clientData: Some(loadedClient)});
    | InputNameChange(newName) =>
      ReasonReact.Update({...state, inputName: Some(newName)})
    | InputSheetChange(newSheet) =>
      ReasonReact.Update({...state, inputSheet: Some(newSheet)})
    | Login =>
      Js.log("CheckLogin in Server: if ture/ else");
      ReasonReact.Update({...state, isLoggedin: true});
    | ClickAnimation =>
      ReasonReact.SideEffects((_self => Js.log("animation")))
    },
  didMount: self => self.send(Login),
  render: self => {
    let clientRow =
      switch (self.state.clientData) {
      | Some(clients) =>
        ReasonReact.array(
          Array.map(
            (clientInfo: ClientData.client) =>
              <Client key={clientInfo.applicationId} clientInfo />,
            clients,
          ),
        )
      | None => <div> {ReasonReact.string("No Data")} </div>
      };

    <div className="App">
      <Graphqltest1 />
      {
        self.state.isLoggedin ?
          <table className="table">
            <thead className="thead-light">
              <tr>
                <th scope="col"> {ReasonReact.string("Name")} </th>
                <th scope="col"> {ReasonReact.string("Summary Sheet")} </th>
                <th scope="col"> {ReasonReact.string("Application ID")} </th>
                <th scope="col"> {ReasonReact.string("Time Created")} </th>
              </tr>
            </thead>
            <tbody>
              <tr>
                <td>
                  <input
                    className="table-input"
                    placeholder="Client Name"
                    onChange={
                      event =>
                        self.send(
                          InputNameChange(
                            ReactEvent.Form.target(event)##value,
                          ),
                        )
                    }
                  />
                </td>
                <td>
                  <input
                    className="table-input"
                    placeholder="Summary Sheet URL"
                    onChange={
                      event =>
                        self.send(
                          InputSheetChange(
                            ReactEvent.Form.target(event)##value,
                          ),
                        )
                    }
                  />
                </td>
                <td>
                  <button
                    className="table-submit-btn"
                    onClick={_event => self.send(Submit(dummyClients))}>
                    {ReasonReact.string("Create New Application")}
                  </button>
                </td>
              </tr>
              clientRow
            </tbody>
          </table> :
          <form>
            <div className="form-group">
              <input
                className="form-control"
                id="login-username"
                placeholder="Username"
              />
            </div>
            <div className="form-group">
              <input
                className="form-control"
                id="login-password"
                placeholder="Password"
              />
            </div>
            <button
              className="btn btn-primary"
              onClick={_event => self.send(Login)}>
              {ReasonReact.string("Submit")}
            </button>
          </form>
      }
    </div>;
  },
};
