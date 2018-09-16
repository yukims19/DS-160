[@bs.module] external gql: ReasonApolloTypes.gql = "graphql-tag";
module GetClientTable = [%graphql
  {|
     query{
     clients{
     id
     name
     dataSheet
     applicationId
     createdAt
     updatedAt
     }
     }
     |}
];

module GetClientTableQuery = ReasonApollo.CreateQuery(GetClientTable);

/*
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
    */

let component = ReasonReact.statelessComponent("ClientTableQuery");

let make = _children => {
  ...component,
  render: _self =>
    <div>
      <GetClientTableQuery>
        ...{
             ({result}) =>
               switch (result) {
               | Loading => <div> {"Loading" |> ReasonReact.string} </div>
               | Error(e) => <div> {ReasonReact.string(e##message)} </div>
               | Data(response) =>
                 switch (response##clients) {
                 | Some(clients) =>
                   <tbody>
                     {
                       clients
                       |> Array.map(clientInfo =>
                            <Client
                              key={string_of_int(clientInfo##id)}
                              clientInfo
                            />
                          )
                       |> ReasonReact.array
                     }
                   </tbody>
                 | None => <div />
                 }
               /*
                ReasonReact.array(
                  Array.map(
                    clientInfo =>
                      <Client key={clientInfo.applicationId} clientInfo />,
                      response##clients,
                  ),
                )*/
               }
           }
      </GetClientTableQuery>
    </div>,
};
