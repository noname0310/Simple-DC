using System;
using System.Collections.Generic;
using SimpleDC;

// ReSharper disable ArrangeObjectCreationWhenTypeNotEvident

DcCalculator dcCalculator = new();

for (;;)
{
    var inputSplit = Console.ReadLine()?.Split(' ');
    if (inputSplit != null)
    {
        foreach (var item in inputSplit)
        {
            var input = Token.LexSingle(item);
            var result = dcCalculator.Step(in input);
            if (result.Message != null)
                Console.WriteLine(result.Result == Result.Ok 
                    ? $"{result.Message}" 
                    : $"Err: {result.Message}");
            else if (result.Result == Result.Err)
                Console.WriteLine("Err");
            if (result.Message == "q")
                Environment.Exit(0);

            if (result.Result == Result.Err)
                break;
        }
    }
}

namespace SimpleDC
{
    internal class DcCalculator
    {
        private readonly Stack<int> _stack;

        public DcCalculator() => _stack = new ();

        public DcResult Step(in Token token)
        {
            DcResult dcResult;

            switch (token.Type)
            {
                case Token.TokenType.Add:
                    dcResult = CheckStackSize();
                    if (dcResult.Result == Result.Err) return dcResult;
                    _stack.Push(_stack.Pop() + _stack.Pop());
                    return new() { Result = Result.Ok };
                case Token.TokenType.Sub:
                    dcResult = CheckStackSize();
                    if (dcResult.Result == Result.Err) return dcResult;
                    _stack.Push(_stack.Pop() - _stack.Pop());
                    return new() { Result = Result.Ok };
                case Token.TokenType.Mul:
                    dcResult = CheckStackSize();
                    if (dcResult.Result == Result.Err) return dcResult;
                    _stack.Push(_stack.Pop() * _stack.Pop());
                    return new() { Result = Result.Ok };
                case Token.TokenType.Div:
                    dcResult = CheckStackSize();
                    if (dcResult.Result == Result.Err) return dcResult;
                    _stack.Push(_stack.Pop() / _stack.Pop());
                    return new() { Result = Result.Ok };
                case Token.TokenType.Print:
                    if (_stack.TryPeek(out var result))
                        return new() {Result = Result.Ok, Message = result.ToString()};
                    else
                        return new() {Result = Result.Ok, Message = "EOF"};
                case Token.TokenType.Quit:
                    return new() { Result = Result.Ok, Message = "q" };
                case Token.TokenType.Value:
                    if (int.TryParse(token.Value, out var result2))
                    {
                        _stack.Push(result2);
                        return new() {Result = Result.Ok};
                    }
                    else
                        return new() {Result = Result.Err, Message = "Invalid token" };
                default:
                    return new() {Result = Result.Err, Message = "Invalid token" };
            }
        }

        private DcResult CheckStackSize()
        {
            if (_stack.Count < 2)
                return new() { Result = Result.Err, Message = "stack.Count < 2" };
            else
                return new() { Result = Result.Ok };
        }

        public readonly struct DcResult
        {
            public Result Result { get; init; }
            public string Message { get; init; }
        }
    }

    internal readonly struct Token
    {
        public string Value { get; init; }
        public TokenType Type { get; init; }

        public static Token LexSingle(string str) => (str = str.Trim()) switch
        {
            "+" => new() {Type = TokenType.Add},
            "-" => new() {Type = TokenType.Sub},
            "*" => new() {Type = TokenType.Mul},
            "/" => new() {Type = TokenType.Div},
            "p" => new() {Type = TokenType.Print},
            "q" => new() {Type = TokenType.Quit},
            _ => new() {Value = str, Type = TokenType.Value}
        };

        public enum TokenType
        {
            Add,
            Sub,
            Mul,
            Div,
            Print,
            Quit,
            Value
        }
    }

    internal enum Result
    {
        Ok,
        Err
    }
}
