using System;
using System.Collections.Generic;
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
            var ok = result.AsOk();
            if (ok.Value != null)
            {
                Console.WriteLine(ok.Value);
                if (ok.Value == "q")
                    Environment.Exit(0);
            }
        }
        else
        {
            var err = result.AsErr();
            Console.WriteLine(err.Error == string.Empty
                ? "Err"
                : $"Err: {err.Error}"
            );
        }
    }
}

namespace SimpleDC
{
    // ReSharper disable once UnusedTypeParameter
    public interface IResult<in T,
    // ReSharper disable once UnusedTypeParameter
        in TE> { }

    public readonly struct Result<T, TE>
    {
        public readonly bool IsOk;
        private readonly T _value;
        private readonly TE _error;

        private Result(bool isOk, T value, TE error)
        {
            IsOk = isOk;
            _value = value;
            _error = error;
        }

        public Ok<T> AsOk()
        {
            if (IsOk)
                return new(_value);
            throw new InvalidCastException();
        }

        public Err<TE> AsErr()
        {
            if (!IsOk)
                return new(_error);
            throw new InvalidCastException();
        }

        public static explicit operator Result<T, TE>(Ok<T> ok) => new(true, ok.Value, default);
        public static explicit operator Result<T, TE>(Err<TE> err) => new(false, default, err.Error);
    }

    public readonly struct Ok<T> : IResult<T, object>
    {
        public T Value { get; }
        public Ok(T value) => Value = value;
    }

    public readonly struct Err<T> : IResult<object, T>
    {
        public T Error { get; }
        public Err(T err) => Error = err;
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
                    return (Result<string, string>)new Ok<string>(null);
                }
                case Token.TokenType.Sub:
                {
                    var dcResult = CheckStackSize();
                    if (!dcResult.IsOk) return dcResult;
                    var first = _stack.Pop();
                    _stack.Push(_stack.Pop() - first);
                    return (Result<string, string>)new Ok<string>(null);
                }
                case Token.TokenType.Mul:
                {
                    var dcResult = CheckStackSize();
                    if (!dcResult.IsOk) return dcResult;
                    _stack.Push(_stack.Pop() * _stack.Pop());
                    return (Result<string, string>)new Ok<string>(null);
                }
                case Token.TokenType.Div:
                {
                    var dcResult = CheckStackSize();
                    if (!dcResult.IsOk) return dcResult;
                    var first = _stack.Pop();
                    _stack.Push(_stack.Pop() / first);
                    return (Result<string, string>)new Ok<string>(null);
                }
                case Token.TokenType.Print:
                    return (Result<string, string>)(_stack.TryPeek(out var result)
                        ? new Ok<string>(result.ToString())
                        : new Ok<string>("EOF"));
                case Token.TokenType.Quit:
                    return (Result<string, string>)new Ok<string>("q");
                case Token.TokenType.Value:
                    if (int.TryParse(token.Value, out var result2))
                    {
                        _stack.Push(result2);
                        return (Result<string, string>)new Ok<string>(null);
                    }
                    else
                        return (Result<string, string>) new Err<string>("Invalid token");
                default:
                    return (Result<string, string>)new Err<string>("Invalid token");
            }
        }

        private Result<string, string> CheckStackSize() => _stack.Count < 2
            ? (Result<string, string>)new Err<string>("stack.Count < 2")
            : (Result<string, string>)new Ok<string>(null);
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
