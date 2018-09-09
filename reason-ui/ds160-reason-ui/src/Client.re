let component = ReasonReact.statelessComponent("Client");

let make = (~clientInfo: ClientData.client, _children) => {
  ... component,
  render: (_self) =>
      <tr>
        <td>(ReasonReact.string(clientInfo.name))</td>
        <td>(ReasonReact.string(clientInfo.sheet))</td>
        <td>(ReasonReact.string(clientInfo.applicationId))</td>
        <td>(ReasonReact.string(clientInfo.time))</td>
      </tr>
};
