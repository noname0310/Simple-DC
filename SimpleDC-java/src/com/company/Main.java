package com.company;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Stack;

public class Main {
    public static void main(String[] args) throws IOException {
        var dcCalculator = new DcCalculator();
        var reader = new BufferedReader(new InputStreamReader(System.in));
        for (;;) {
            var inputSplit = reader.readLine().split(" ");
            for (var item : inputSplit) {
                var input = Token.lexSingle(item);
                var result = dcCalculator.step(input);

                if (result instanceof Ok) {
                    @SuppressWarnings("unchecked")
                    var okResult = (Ok<String>)result;
                    if (okResult.getValue() != null) {
                        System.out.println("Ok: " + okResult.getValue());
                        if (okResult.getValue().equals("q"))
                            System.exit(0);
                    }
                } else if (result instanceof Err) {
                    @SuppressWarnings("unchecked")
                    var errResult = (Err<String>)result;
                    if (errResult.getErr() != null)
                        System.out.println("Err: " + errResult.getErr());
                    else
                        System.out.println("Err");
                }
            }
        }
    }
}

abstract class Result {}

class Ok<T> extends Result {
    private final T value;

    public Ok(T value) {
        this.value = value;
    }

    public T getValue(){
        return value;
    }
}

class Err<E> extends Result {
    private final E err;

    public Err(E err) {
        this.err = err;
    }

    public E getErr() {
        return err;
    }
}

class DcCalculator
{
    private final Stack<Integer> stack;

    public DcCalculator() {
        stack = new Stack<>();
    }

    public Result step(Token token)
    {
        if (token instanceof Token.Add) {
            var dcResult = CheckStackSize();
            if (dcResult instanceof Err) return dcResult;
            stack.push(stack.pop() + stack.pop());
            return new Ok<String>(null);
        } else if (token instanceof Token.Sub) {
            var dcResult = CheckStackSize();
            if (dcResult instanceof Err) return dcResult;
            stack.push(stack.pop() - stack.pop());
            return new Ok<String>(null);
        } else if (token instanceof Token.Mul) {
            var dcResult = CheckStackSize();
            if (dcResult instanceof Err) return dcResult;
            stack.push(stack.pop() * stack.pop());
            return new Ok<String>(null);
        } else if (token instanceof Token.Div) {
            var dcResult = CheckStackSize();
            if (dcResult instanceof Err) return dcResult;
            stack.push(stack.pop() / stack.pop());
            return new Ok<String>(null);
        } else if (token instanceof Token.Print) {
            if (stack.size() != 0)
                return new Ok<>(stack.peek().toString());
            else
                return new Ok<>("EOF");
        } else if (token instanceof Token.Quit) {
            return new Ok<>("q");
        } else if (token instanceof Token.Value) {
            try {
                var parsedInt = Integer.parseInt(((Token.Value) token).getValue());
                stack.push(parsedInt);
                return new Ok<String>(null);
            } catch (NumberFormatException e) {
                return new Err<>("Invalid token");
            }
        } else
            return new Err<>("Invalid token");
    }

    private Result CheckStackSize()
    {
        if (stack.size() < 2)
            return new Err<>("stack.Count < 2");
        else
            return new Ok<String>(null);
    }
}

class Token
{
    static class Add extends Token{}
    static class Sub extends Token{}
    static class Mul extends Token{}
    static class Div extends Token{}
    static class Print extends Token{}
    static class Quit extends Token{}
    static class Value extends Token{
        private final String value;

        public Value(String value) {
            this.value = value;
        }

        public String getValue() {
            return value;
        }
    }

    public static Token lexSingle(String str) {
        str = str.trim();
        switch (str) {
            case "+":
                return new Add();
            case "-":
                return new Sub();
            case "*":
                return new Mul();
            case "/":
                return new Div();
            case "p":
                return new Print();
            case "q":
                return new Quit();
            default:
                return new Value(str);
        }
    }
}