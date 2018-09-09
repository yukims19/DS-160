[%bs.raw {|require('./App.css')|}];
type state = {clientData: option(array(ClientData.client))};
type action =
  | Loaded(array(ClientData.client));

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
}
|]

let make = (_children) => {
  ...component,
  initialState: () =>{
    clientData: Some(dummyClients)
  },
  reducer: (action, _state) =>{
    switch action{
    | Loaded(loadedClient) =>
      ReasonReact.Update({
      clientData: Some(loadedClient)
    })
    }
  },
  render: (self) =>{
    let loadReposButton =
    <button onClick=((_event) => self.send(Loaded(dummyClients)))>
        {ReasonReact.string("Load Repos")}
  </button>;

    let clientRow =
      switch(self.state.clientData){
      | Some(clients) => ReasonReact.array(
          Array.map(
        (clientInfo: ClientData.client) => <Client key=clientInfo.applicationId clientInfo=clientInfo />,
        clients
        )
      )
      | None => loadReposButton
      };

    <div className="App">
        (ReasonReact.string("Hello World"))
  <table className="table">
    <thead>
      <tr>
        <th scope="col">(ReasonReact.string("Name"))</th>
        <th scope="col">(ReasonReact.string("Summary Sheet"))</th>
        <th scope="col">(ReasonReact.string("Application ID"))</th>
        <th scope="col">(ReasonReact.string("Time Created"))</th>
      </tr>
    </thead>
    <tbody>
      <tr>
        <td><input className="table-input" placeholder="Client Name"/></td>
        <td><input className="table-input" placeholder="Summary Sheet URL"/></td>
        <td><button className="table-submit-btn">(ReasonReact.string("Create New Application"))</button></td>
      </tr>
     {clientRow}
    </tbody>
  </table>
</div>
}
};
