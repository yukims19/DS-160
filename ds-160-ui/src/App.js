import React, { Component } from "react";
import "./App.css";
import { Table, Button, Input } from "antd";

class App extends Component {
  constructor(props) {
    super(props);
    this.state = {
      inputName: null,
      inputSheet: null
    };
    this.columns = [
      {
        title: "Name",
        dataIndex: "name",
        key: "name",
        render: (text, record) =>
          <span>
            {record.key === "0"
              ? <Input
                  className="table-input"
                  placeholder="Client Name"
                  onChange={e =>
                    this.handleInputChange("inputName", e.target.value)}
                />
              : record.name}
          </span>
      },
      {
        title: "Summary Sheet",
        dataIndex: "sheet",
        key: "sheet",
        render: (text, record) =>
          <span>
            {record.key === "0"
              ? <Input
                  className="table-input"
                  placeholder="Summary Sheet URL"
                  onChange={e =>
                    this.handleInputChange("inputSheet", e.target.value)}
                />
              : record.sheet}
          </span>
      },
      {
        title: "Application ID",
        dataIndex: "applicationId",
        key: "applicationId",
        render: (text, record) =>
          <span>
            {record.key === "0"
              ? <Button
                  type="primary"
                  onClick={() => this.handleClickNew()}
                  className="table-submit-btn"
                >
                  Create New Application
                </Button>
              : record.applicationId}
          </span>
      },
      {
        title: "Time created",
        dataIndex: "time",
        key: "time",
        render: (text, record) =>
          <span>
            {record.key === "0" ? "" : record.time}
          </span>
      }
    ];

    this.data = [
      {
        key: "0",
        name: "John Brown",
        sheet: "https://google.com.example",
        applicationId: "A123456",
        time: "9/1/2018"
      },
      {
        key: "1",
        name: "John Brown",
        sheet: "https://google.com.example",
        applicationId: "A123456",
        time: "9/1/2018"
      },
      {
        key: "2",
        name: "John Brown",
        sheet: "https://google.com.example",
        applicationId: "A123456",
        time: "9/1/2018"
      },
      {
        key: "3",
        name: "John Brown",
        sheet: "https://google.com.example",
        applicationId: "A123456",
        time: "9/1/2018"
      }
    ];
  }

  handleInputChange(inputKey, inputValue) {
    this.setState({
      [inputKey]: [inputValue]
    });
  }
  handleClickNew() {
    console.log("clicked", this.state.inputName, this.state.inputSheet);
  }
  render() {
    return (
      <div className="App">
        <div className="main-table">
          <Table columns={this.columns} dataSource={this.data} />
        </div>
      </div>
    );
  }
}

export default App;
