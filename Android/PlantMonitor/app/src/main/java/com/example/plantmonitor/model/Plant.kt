package com.example.plantmonitor.model


data class Plant(val stringResourceId: Int, val plantName: String)

{


    @Override
    override fun toString(): String {
        return plantName
    }
}
