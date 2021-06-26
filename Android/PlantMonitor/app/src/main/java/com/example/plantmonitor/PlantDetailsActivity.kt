package com.example.plantmonitor

import android.content.ContentValues.TAG
import android.os.Bundle
import android.util.Log
import android.widget.CompoundButton
import android.widget.Switch
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import com.github.mikephil.charting.charts.Chart
import com.github.mikephil.charting.charts.LineChart
import com.github.mikephil.charting.data.Entry
import com.github.mikephil.charting.data.LineData
import com.github.mikephil.charting.data.LineDataSet
import com.google.firebase.database.DataSnapshot
import com.google.firebase.database.DatabaseError
import com.google.firebase.database.FirebaseDatabase
import com.google.firebase.database.ValueEventListener


class PlantDetailsActivity : AppCompatActivity()  {

    companion object {

        const val PLANT = "plant"

    }

    lateinit var waterPumpSwitch: Switch

    lateinit var automaticWateringSwitch: Switch

    lateinit var waterLevelIndicator: TextView

    lateinit var moistureLevelIndicator: TextView

    lateinit var lineChart: LineChart


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.plantdetails_activity)

        //val plant = "medronho2"
        val plant = intent?.extras?.getString(PLANT).toString()

        val database = FirebaseDatabase.getInstance()
        val myRef = database.getReference("plants")


        val entries = mutableListOf<Entry>()

        entries.add(Entry(1f, 10f))
        entries.add(Entry(2f, 2f))
        entries.add(Entry(3f, 7f))
        entries.add(Entry(4f, 20f))
        entries.add(Entry(5f, 16f))

        val v1 = LineDataSet(entries, "My type")

        v1.setDrawValues(false)
        v1.setDrawFilled(true)
        v1.lineWidth = 3f
        v1.fillColor = R.color.black
        v1.fillAlpha = R.color.black

        lineChart = findViewById(R.id.lineChart)

        lineChart.setTouchEnabled(true)
        lineChart.setPinchZoom(true)

        lineChart.data = LineData(v1)
        // Retrieve a binding object that allows you to refer to views by id name
        // Names are converted from snake case to camel case.
        // For example, a View with the id word_one is referenced as binding.wordOne
        //val binding = ActivityDetailBinding.inflate(layoutInflater)
        //setContentView(binding.root)

        waterPumpSwitch = findViewById(R.id.waterpumpswitch)

        automaticWateringSwitch = findViewById(R.id.aumaticwateringswitch)

        waterLevelIndicator = findViewById(R.id.waterlevel)

        moistureLevelIndicator = findViewById(R.id.moisture)


        // Read from the database
        // Read from the database
        myRef.addValueEventListener(object : ValueEventListener {
            override fun onDataChange(dataSnapshot: DataSnapshot) {
                // This method is called once with the initial value and again
                // whenever data at this location is updated.
                val value = dataSnapshot.child("waterlevel").getValue(Long::class.java)
                if (value != null)
                {
                    changeWaterLevel(value.toString())
                }
                Log.d(TAG, "Value is: $value")

                val moistureValue = dataSnapshot.child(plant).child("moisture").getValue(Long::class.java)
                if (moistureValue != null)
                {
                    changeMoistureLevel(moistureValue.toString())
                }

                val waterPumpValue = dataSnapshot.child(plant).child("waterpump").getValue(Boolean::class.java)
                if (waterPumpValue != null)
                {
                    changePumpValue(waterPumpValue)
                }
                Log.d(TAG, "Water Pump Value is: $waterPumpValue")

                val automaticValue = dataSnapshot.child(plant).child("automaticwatering").getValue(Boolean::class.java)
                if (automaticValue != null)
                {
                    changeAutomaticWateringValue(automaticValue)
                }
                Log.d(TAG, "Automatic Watering Value is: $automaticValue")
            }

            override fun onCancelled(error: DatabaseError) {
                // Failed to read value
                Log.w(TAG, "Failed to read value.", error.toException())
            }
        })


        automaticWateringSwitch.setOnCheckedChangeListener { _, isChecked ->
            myRef.child(plant).child("automaticwatering").setValue(isChecked)
        }

        waterPumpSwitch.setOnCheckedChangeListener { _, isChecked ->
            myRef.child(plant).child("waterpump").setValue(isChecked)
        }


    }

    private fun changePumpValue(waterPumpValue: Boolean) {
            waterPumpSwitch.isChecked = waterPumpValue

    }


    private fun changeAutomaticWateringValue(automaticValue: Boolean) {
        automaticWateringSwitch.isChecked = automaticValue

    }

    private fun changeWaterLevel( value:String) {

        waterLevelIndicator.text = "Water Level :" + value + "%"
    }

    private fun changeMoistureLevel( value:String) {

        moistureLevelIndicator.text = "Soil Moisture Level :" + value + "%"
    }

}