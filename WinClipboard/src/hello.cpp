// hello.cpp
#include <node.h>

namespace demo {

	using v8::Exception;
	using v8::FunctionCallbackInfo;
	using v8::Isolate;
	using v8::Local;
	using v8::Object;
	using v8::String;
	using v8::Value;
	using v8::Number;

	void PrintHello(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();
		args.GetReturnValue().Set(String::NewFromUtf8(isolate, "Hello world!"));
	}

	// 这是 "add" 方法的实现
	// 输入参数使用 const FunctionCallbackInfo<Value>& args 结构传入
	void Add(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();

		// 检查传入的参数的个数
		if (args.Length() < 2) {
			// 抛出一个错误并传回到 JavaScript
			isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "参数数量错误")));
			return;
		}

		// 检查参数的类型
		if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate, "参数错误")));
			return;
		}

		// 执行操作
		double value = args[0]->NumberValue() + args[1]->NumberValue();
		Local<Number> num = Number::New(isolate, value);

		// 设置返回值
		args.GetReturnValue().Set(num);
	}

	void Init(Local<Object> exports) {
		NODE_SET_METHOD(exports, "print", PrintHello);
		NODE_SET_METHOD(exports, "add", Add);
	}

	//NODE_MODULE(NODE_GYP_MODULE_NAME, Init)

}  // namespace demo