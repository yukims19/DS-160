[@bs.module] external gql: ReasonApolloTypes.gql = "graphql-tag";
module AddNewClient = [%graphql
  {|
     mutation($userId: String!, $name: String!, $dataSheet: String!) {
     addNewClient(dataSheet: $dataSheet, userId: $userId, name: $name)
     }
     |}
];

module AddNewClientMutation = ReasonApollo.CreateMutation(AddNewClient);
open BsReactstrap;

let component = ReasonReact.statelessComponent("AddNewClientQuery");

let make = (~clientName, ~summarySheet, _children) => {
  ...component,
  render: _ => {
    let name =
      switch (clientName) {
      | Some(name) => name
      | None => ""
      };
    let dataSheet =
      switch (summarySheet) {
      | Some(name) => name
      | None => ""
      };
    <AddNewClientMutation>
      ...{
           (mutation, _) => {
             let getClientTableQuery =
               AddNewClient.make(~userId="1", ~name, ~dataSheet, ());
             <button
               className="table-submit-btn"
               onClick={
                 _e =>
                   if (String.length(name) > 0
                       && String.length(dataSheet) > 0) {
                     mutation(
                       ~variables=getClientTableQuery##variables,
                       ~refetchQueries=[|"clients"|],
                       (),
                     )
                     |> ignore;
                   } else {
                     %bs.raw
                     {| alert('Enter Client Name and Summary Sheet URL') |};
                   }
               }>
               {ReasonReact.string("Create New Application Mutation")}
             </button>;
           }
         }
    </AddNewClientMutation>;
  },
};
