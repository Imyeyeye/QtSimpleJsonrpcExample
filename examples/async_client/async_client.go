package main

import (
	"fmt"
	"net/rpc/jsonrpc"
)

type Args struct {
	A int
	B int
}

type Addition_Result struct {
	Result int
}

type Division_Result struct {
	Result float32
}

func main() {

	wait := make(chan bool, 1)

	client, err := jsonrpc.Dial("tcp", "127.0.0.1:1234")
	defer client.Close()
	if err != nil {
		fmt.Println("dialing:", err.Error())
		//return
	}

	args := Args{4, 5}

	//加法
	var addition_result Addition_Result
	addition_call := client.Go("Rpc_Call_Function.Addition", args, &addition_result, nil)
	additionCall := <-addition_call.Done
	if additionCall.Error != nil {
		fmt.Println("Addition error:", addition_call.Error)
	} else {
		fmt.Printf("Addition: %d + %d = %d \r\n", args.A, args.B, addition_result.Result)
	}

	//除法
	var division_result Division_Result
	division_call := client.Go("Rpc_Call_Function.Division", args, &division_result, nil)
	divisionCall := <-division_call.Done
	if divisionCall.Error != nil {
		fmt.Println("Division error:", division_call.Error)
	} else {
		fmt.Printf("Division: %d / %d = %f \r\n", args.A, args.B, division_result.Result)
	}

	<-wait
}
