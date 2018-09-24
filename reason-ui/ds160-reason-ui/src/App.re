[%bs.raw {|require('./App.css')|}];

type result = {data: user}
and user = {users: bool};

type state = {
  clientData: option(array(ClientData.client)),
  inputName: option(string),
  inputSheet: option(string),
};
type action =
  | UpdateClients(array(ClientData.client))
  | InputNameChange(string)
  | InputSheetChange(string)
  | ClickAnimation;

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  initialState: () => {
    clientData: Some([||]),
    inputName: None,
    inputSheet: None,
  },
  reducer: (action, state) =>
    switch (action) {
    | UpdateClients(loadedClient) =>
      Js.log("clicked");
      ReasonReact.Update({...state, clientData: Some(loadedClient)});
    | InputNameChange(newName) =>
      ReasonReact.Update({...state, inputName: Some(newName)})
    | InputSheetChange(newSheet) =>
      ReasonReact.Update({...state, inputSheet: Some(newSheet)})
    | ClickAnimation =>
      ReasonReact.SideEffects((_self => Js.log("animation")))
    },
  render: self =>
    <div className="App">
      <LoginGuard>
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
                <AddNewClientMutation
                  clientName={self.state.inputName}
                  summarySheet={self.state.inputSheet}
                  updateClientRows={
                    clientRows => self.send(UpdateClients(clientRows))
                  }
                />
              </td>
            </tr>
            <ClientTableQuery
              updateClientRows={
                clientRows => self.send(UpdateClients(clientRows))
              }
            />
          </tbody>
        </table>
      </LoginGuard>
    </div>,
};
/*
 ReasonReact.UpdateWithSideEffects(
   state,
   (
     _self => {
       let payload = Js.Dict.empty();
       Js.Dict.set(
         payload,
         "query",
         Js.Json.string(
           "query {\n  users(username: \"yuk\", password: \"123\")\n}\n",
         ),
       );
       Js.Promise.(
         Fetch.fetchWithInit(
           "http://localhost:8082/dynamic",
           Fetch.RequestInit.make(
             ~method_=Post,
             ~body=
               Fetch.BodyInit.make(
                 Js.Json.stringify(Js.Json.object_(payload)),
               ),
             ~headers=
               Fetch.HeadersInit.makeWithArray([|
                 ("content-type", "application/json"),
               |]),
             (),
           ),
         )
         |> then_(Fetch.Response.json)
         |> then_(json =>
              Js.log("Success:" ++ Js_json.stringify(json)) |> resolve
            )
         |> catch(err => Js.Promise.resolve(Js.log(err)))
         |> ignore
       );
     }
   ),
 )*/

/*
 didMount: self => {
 let client = ClientApollo.instance;
 let gql = ClientApollo.gql;
 let allOrdersMeta = GetOrdersMeta.make();
 let query: ApolloClient.queryObj = {
 "query": gql(. allOrdersMeta##query),
 "variables": allOrdersMeta##variables,
 };
 let apolloData = apolloClient##query(query);
 ();
 },*/

/*
 let _clientRow =
 switch (self.state.clientData) {
 | Some(clients) =>
 ReasonReact.array(
 Array.map(
 (clientInfo: ClientData.client) =>
 <Client key=clientInfo##applicationId clientInfo />,
 clients,
 ),
 )
 | None => <div> {ReasonReact.string("No Data")} </div>
 };*/
