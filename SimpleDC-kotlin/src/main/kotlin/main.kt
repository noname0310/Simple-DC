import java.util.*

fun main() {
    val dcCalculator = DcCalculator()
    while(true) {
        val inputSplit = readLine()?.split(' ') ?: continue
        for(item in inputSplit) {
            val input = Token.lexSingle(item)
            when (val result = dcCalculator.step(input)) {
                is Ok -> {
                    if (result.value != null) {
                        println(result.value)
                        if (result.value == "q") {
                            kotlin.system.exitProcess(0)
                        }
                    }
                }
                is Err -> println(
                    if (result.error == null)
                        "Err"
                    else
                        "Err: ${result.error}"
                )
            }
        }
    }
}

sealed class Result<out T, out E>
class Ok<out T>(val value: T) : Result<T, Nothing>()
class Err<out E>(val error: E) : Result<Nothing, E>()

class DcCalculator {
    private val stack: Stack<Int> = Stack()

    fun step(token: Token): Result<String?, String?> = when (token) {
        is Token.Add -> {
            val result = checkStackSize()
            if (result is Err) result
            else {
                stack.push(stack.pop() + stack.pop())
                Ok(null)
            }
        }
        is Token.Sub -> {
            val result = checkStackSize()
            if (result is Err) result
            else {
                stack.push(stack.pop() - stack.pop())
                Ok(null)
            }
        }
        is Token.Mul -> {
            val result = checkStackSize()
            if (result is Err) result
            else {
                stack.push(stack.pop() * stack.pop())
                Ok(null)
            }
        }
        is Token.Div -> {
            val result = checkStackSize()
            if (result is Err) result
            else {
                stack.push(stack.pop() / stack.pop())
                Ok(null)
            }
        }
        is Token.Print -> {
            if (0 < stack.count())
                Ok(stack.peek().toString())
            else
                Ok("EOF")
        }
        is Token.Quit -> Ok("q")
        is Token.Value -> {
            val parsedResult = token.value.toIntOrNull()
            if (parsedResult != null) {
                stack.push(parsedResult)
                Ok(null)
            } else
                Err("Invalid token")
        }
    }

    private fun checkStackSize(): Result<String?, String?> =
        if (stack.count() < 2) Err("stack.Count < 2")
        else Ok(null)
}

sealed class Token {
    object Add : Token()
    object Sub : Token()
    object Mul : Token()
    object Div : Token()
    object Print : Token()
    object Quit : Token()
    class Value(val value: String) : Token()
    companion object {
        fun lexSingle(str: String): Token = when (val trimStr = str.trim()) {
            "+" -> Add
            "-" -> Sub
            "*" -> Mul
            "/" -> Div
            "p" -> Print
            "q" -> Quit
            else -> Value(trimStr)
        }
    }
}
