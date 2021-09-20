package io.eontimer.util.javafx

import javafx.beans.property.ObjectProperty
import javafx.collections.FXCollections
import javafx.scene.control.ChoiceBox
import javafx.util.StringConverter
import kotlin.reflect.KClass

interface Choice {
    val displayName: String
}

class ChoiceConverter<T>(
    type: KClass<T>
) : StringConverter<T>()
    where T : Enum<T>,
          T : Choice {
    private val displayNames by lazy {
        type.java.enumConstants.associateBy { it.displayName }
    }

    override fun toString(value: T): String =
        value.displayName

    override fun fromString(string: String): T =
        displayNames[string] ?: error("unable to find Choice for $string")
}

class ChoiceField<T>(
    choiceBox: ChoiceBox<T>
) where T : Enum<T>,
        T : Choice {
    val valueProperty: ObjectProperty<T> = choiceBox.valueProperty()
    var value: T by valueProperty
}

inline fun <reified T> ChoiceBox<T>.asChoiceField(): ChoiceField<T>
    where T : Enum<T>,
          T : Choice {
    val choices = T::class.java.enumConstants
    items = FXCollections.observableArrayList(*choices)
    converter = ChoiceConverter(T::class)
    return ChoiceField(this)
}
