[@bs.module] external gql: ReasonApolloTypes.gql = "graphql-tag";
module GetTopStories = [%graphql
  {|
     query{
     hackerNews {
     topStory {
     id
     title
     url
     }
     }
     }
|}
];

module GetTopStroriesQuery = ReasonApollo.CreateQuery(GetTopStories);

let component = ReasonReact.statelessComponent("HackerNewsTopStory");

let make = _children => {
  ...component,
  render: _self =>
    <div>
      <GetTopStroriesQuery>
        ...{
             ({result}) =>
               switch (result) {
               | Loading => <div> {"Loading" |> ReasonReact.string} </div>
               | Error(e) => <div> {ReasonReact.string(e##message)} </div>
               | Data(response) =>
                 <table>
                   <tbody>
                     {
                       Js.Array.map(
                         story =>
                           <tr> {story##title |> ReasonReact.string} </tr>,
                         response##hackerNews##topStory,
                       )
                       |> ReasonReact.array
                     }
                   </tbody>
                 </table>
               }
           }
      </GetTopStroriesQuery>
    </div>,
};
