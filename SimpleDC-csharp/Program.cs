using System;
using System.Collections.Generic;
using System.Net;
using System.Xml.XPath;
using Microsoft.VisualBasic.CompilerServices;
using SimpleDC;

// ReSharper disable ArrangeObjectCreationWhenTypeNotEvident

DcCalculator dcCalculator = new();

for (;;)
{
    var inputSplit = Console.ReadLine()?.Split(' ');
    if (inputSplit == null)
        continue;
    foreach (var item in inputSplit)
    {
        var input = Token.LexSingle(item);
        var result = dcCalculator.Step(in input);
        if (result.IsOk)
        {
            if (result.Value != null)
            {
                Console.WriteLine(result.Value);
                if (result.Value == "q")
                    Environment.Exit(0);
            }
        }
        else
        {
            Console.WriteLine(result.Error == string.Empty
                ? "Err"
                : $"Err: {result.Error}"
            );
        }
    }
}

namespace SimpleDC
{
    public readonly struct Result<T> where T : struct
    {
        public readonly bool IsOk;
        public bool IsErr => !IsOk;
        private readonly T _value;
        private readonly Exception _error;

        private Result(bool isOk, T value, Exception exception)
        {
            IsOk = isOk;
            _value = value;
            _error = exception;
        }

        public T? Value
        {
            get
            {
                if (IsOk)
                    return _value;
                return null;
            }
        }

        public Exception Error
        {
            get
            {
                if (!IsOk)
                    return _error;
                throw new InvalidOperationException();
            }
        }

        public static Result<T> Ok(T ok) => new(true, ok, null);
        public static Result<T> Err(Exception error) => new(false, default, error);
    }

    public readonly struct Result<T, TE>
    {
        public readonly bool IsOk;
        public bool IsErr => !IsOk;
        private readonly T _value;
        private readonly TE _error;

        private Result(bool isOk, T value, TE error)
        {
            IsOk = isOk;
            _value = value;
            _error = error;
        }

        public T Value
        {
            get
            {
                if (IsOk)
                    return _value;
                throw new InvalidOperationException();
            }
        }

        public TE Error
        {
            get
            {
                if (!IsOk)
                    return _error;
                throw new InvalidOperationException();
            }
        }

        public static Result<T, TE> Ok(T ok) => new(true, ok, default);
        public static Result<T, TE> Err(TE err) => new(false, default, err);
    }

    internal class DcCalculator
    {
        private readonly Stack<int> _stack;

        public DcCalculator() => _stack = new ();

        public Result<string, string> Step(in Token token)
        {
            switch (token.Type)
            {
                case Token.TokenType.Add:
                {
                    var dcResult = CheckStackSize();
                    if (!dcResult.IsOk) return dcResult;
                    _stack.Push(_stack.Pop() + _stack.Pop());
                    return Result<string, string>.Ok(null);
                }
                case Token.TokenType.Sub:
                {
                    var dcResult = CheckStackSize();
                    if (!dcResult.IsOk) return dcResult;
                    var first = _stack.Pop();
                    _stack.Push(_stack.Pop() - first);
                    return Result<string, string>.Ok(null);
                }
                case Token.TokenType.Mul:
                {
                    var dcResult = CheckStackSize();
                    if (!dcResult.IsOk) return dcResult;
                    _stack.Push(_stack.Pop() * _stack.Pop());
                    return Result<string, string>.Ok(null);
                }
                case Token.TokenType.Div:
                {
                    var dcResult = CheckStackSize();
                    if (!dcResult.IsOk) return dcResult;
                    var first = _stack.Pop();
                    _stack.Push(_stack.Pop() / first);
                    return Result<string, string>.Ok(null);
                }
                case Token.TokenType.Print:
                    return _stack.TryPeek(out var result)
                        ? Result<string, string>.Ok(result.ToString())
                        : Result<string, string>.Ok("EOF");
                case Token.TokenType.Quit:
                    return Result<string, string>.Ok("q");
                case Token.TokenType.Value:
                    if (int.TryParse(token.Value, out var result2))
                    {
                        _stack.Push(result2);
                        return Result<string, string>.Ok(null);
                    }
                    else
                        return Result<string, string>.Err("Invalid token");
                default:
                    return Result<string, string>.Err("Invalid token");
            }
        }

        private Result<string, string> CheckStackSize() => _stack.Count < 2
            ? Result<string, string>.Err("stack.Count < 2")
            : Result<string, string>.Err(null);
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
}
