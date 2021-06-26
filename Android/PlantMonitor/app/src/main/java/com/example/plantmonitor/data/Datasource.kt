package com.example.plantmonitor.data

import com.example.plantmonitor.R
import com.example.plantmonitor.model.Plant

class Datasource {

    fun loadPlants(): List<Plant> {
        return listOf<Plant>(
            Plant(1,"medronho1"),
            Plant(2,"medronho2"),
            Plant(3,"medronho3"),

        )
    }
}